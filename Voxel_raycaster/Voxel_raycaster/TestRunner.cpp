#include "TestRunner.h"
#include <iomanip>
#include "Renderers/LODColorTree4DRenderer.h"
#include "PrintStatusBar.h"
#include "Renderers/LODNormalTree4DRenderer.h"


TestRunner::TestRunner(Camera* camera, CameraController* camera_controller, FileFormat* inputformat, RendererManager4D* render_manager4D, Tree4D* tree4D, RenderContext* rc, unsigned char* renderdata) :
	tree4D(tree4D), camera(camera), camera_controller(camera_controller), inputformat(inputformat), rmanager4D(render_manager4D), rc(rc), renderdata(renderdata), nb_of_test_iterations(10)
{

}

double TestRunner::render_nb_of_iterations() const
{
	Tree4DRenderer* currentRenderer = rmanager4D->getCurrentRenderer();

	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	currentRenderer->Render(*rc, tree4D, renderdata, camera_controller->time_point);


	PerformanceTimer timer;
	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		// timed render
		memset(renderdata, 0, rc->n_x*rc->n_y * 4);
		timer.start();
		currentRenderer->Render(*rc, tree4D, renderdata, camera_controller->time_point);
		timer.stop();
	}
	return timer.elapsed_time_milliseconds / nb_of_test_iterations;

}

void TestRunner::run_tests_LOD_work(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string output_data_filename_without_extension)
{
	camera->eye = camera_eye; 

	rmanager4D->setCurrentRenderer("LODWork");
	double LOD_work_mean_time = render_nb_of_iterations();
	size_t LOD_work_smallest_stack_size = LODWorkTree4DRenderer::smallest_stack_size;
	size_t LOD_work_largest_stack_size = LODWorkTree4DRenderer::largest_stack_size;
	data_writer_ptr->writeToFile_endl("LOD_work_mean_time" + suffix + "," + to_string(LOD_work_mean_time));
	data_writer_ptr->writeToFile_endl("LOD_work_smallest_stack_size" + suffix + "," + to_string(LOD_work_smallest_stack_size));
	data_writer_ptr->writeToFile_endl("LOD_work_largest_stack_size" + suffix + "," + to_string(LOD_work_largest_stack_size));


	//write image to disk
	string filename_image = output_data_filename_without_extension + "_LODWork" + suffix;
	writePPM(rc->n_x, rc->n_y, renderdata, filename_image);
}

void TestRunner::run_tests_LOD_work_optimized(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string output_data_filename_without_extension)
{

	double frustrum_width = rc->frustrum->right - rc->frustrum->left;
	double pixel_diameter = frustrum_width / rc->n_x;
	double pixel_radius = 0.5 * pixel_diameter;
	double pixel_size = pixel_radius * pixel_radius; // * PI

	camera->eye = camera_eye;

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer("LODWork"));

	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	lodwr->Render_optimized(*rc, tree4D, renderdata, camera_controller->time_point, pixel_size);


	PerformanceTimer timer;
	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		// timed render
		memset(renderdata, 0, rc->n_x*rc->n_y * 4);
		timer.start();
		lodwr->Render_optimized(*rc, tree4D, renderdata, camera_controller->time_point, pixel_size);
		timer.stop();
	}
	double LOD_work_mean_time  = timer.elapsed_time_milliseconds / nb_of_test_iterations;

	// render again to get largest and smallest stack sizes
	lodwr->Render(*rc, tree4D, renderdata, camera_controller->time_point);

	size_t LOD_work_smallest_stack_size = LODWorkTree4DRenderer::smallest_stack_size;
	size_t LOD_work_largest_stack_size = LODWorkTree4DRenderer::largest_stack_size;
	data_writer_ptr->writeToFile_endl("LOD_work_mean_time" + suffix + "," + to_string(LOD_work_mean_time));
	data_writer_ptr->writeToFile_endl("LOD_work_smallest_stack_size" + suffix + "," + to_string(LOD_work_smallest_stack_size));
	data_writer_ptr->writeToFile_endl("LOD_work_largest_stack_size" + suffix + "," + to_string(LOD_work_largest_stack_size));


	//write image to disk
	string filename_image = output_data_filename_without_extension + "_LODWork" + suffix;
	writePPM(rc->n_x, rc->n_y, renderdata, filename_image);
}

void TestRunner::run_tests_work(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string output_data_filename_without_extension)
{

	camera_eye = camera_eye;

	rmanager4D->setCurrentRenderer("Work");
	//render once to get max amount of work in steps
	double work_far_mean_time = render_nb_of_iterations();
	data_writer_ptr->writeToFile_endl("work_mean_time" + suffix + "," + to_string(work_far_mean_time));


	// write image to disk
	string filename_image = output_data_filename_without_extension + "_Work" + suffix;
	writePPM(rc->n_x, rc->n_y, renderdata, filename_image);
}

void TestRunner::run_tests_sequence(unique_ptr<DataWriter>& data_writer_ptr, string rendererID, string output_data_filename_without_extension, double time_step)
{
	rmanager4D->setCurrentRenderer(rendererID);

	PerformanceTimer timer;
	string sequence_number_suffix;
	int nbOfDigitsInDecimal_gridsize = log10(tree4D->gridsize_T) + 1;
	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		cout << "  iteration " << to_string(i + 1) << " of " << to_string(nb_of_test_iterations) << endl;

		for (int time_i = 0; time_i <= tree4D->gridsize_T; time_i++) {

			showProgressBar(time_i, tree4D->gridsize_T, 10);
			float current_time_point = 0.0 + time_i * time_step;
			// timed render
			memset(renderdata, 0, rc->n_x*rc->n_y * 4);
			timer.start();
			rmanager4D->getCurrentRenderer()->Render(*rc, tree4D, renderdata, current_time_point);
			timer.stop();

			if (i == nb_of_test_iterations - 1)
			{
				stringstream ss;
				ss << setw(nbOfDigitsInDecimal_gridsize) << setfill('0') << time_i;
				sequence_number_suffix = ss.str();
				string filename = output_data_filename_without_extension + "_" +rendererID + "_seq" + sequence_number_suffix;
				writePPM(rc->n_x, rc->n_y, renderdata, filename);
			}
		}
	}
	double Color_sequence_mean_time = timer.elapsed_time_milliseconds / nb_of_test_iterations;
	data_writer_ptr->writeToFile_endl(rendererID + "_seq_mean_time," + to_string(Color_sequence_mean_time));
}

void TestRunner::run_tests_sequence_optimized(unique_ptr<DataWriter>& data_writer_ptr, string rendererID, string suffix, string output_data_filename_without_extension, double time_step)
{
	double frustrum_width = rc->frustrum->right - rc->frustrum->left;
	double pixel_diameter = frustrum_width / rc->n_x;
	double pixel_radius = 0.5 * pixel_diameter;
	double pixel_size = pixel_radius * pixel_radius; // * PI

	LODColorTree4DRenderer* lodcr = dynamic_cast<LODColorTree4DRenderer*>(rmanager4D->getRenderer(rendererID));


	PerformanceTimer timer;
	string sequence_number_suffix;
	int nbOfDigitsInDecimal_gridsize = log10(tree4D->gridsize_T) + 1;
	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		cout << "  iteration " << to_string(i + 1) << " of " << to_string(nb_of_test_iterations) << endl;

		float current_time_point = 0.0;
		float image_counter = 0.0;
		while (current_time_point <= tree4D->gridsize_T){
			 
			showProgressBar(static_cast<int>(current_time_point), static_cast<int>(tree4D->gridsize_T), 10);
			// timed render
			memset(renderdata, 0, rc->n_x*rc->n_y * 4);
			timer.start();
			lodcr->Render_optimized(*rc, tree4D, renderdata, current_time_point, pixel_size);
			timer.stop();

			if (i == nb_of_test_iterations - 1)
			{
				stringstream ss;
				ss << setw(nbOfDigitsInDecimal_gridsize) << setfill('0') << image_counter;
				sequence_number_suffix = ss.str();
				string filename = output_data_filename_without_extension + "_" + rendererID + suffix + "_seq" + sequence_number_suffix;
				writePPM(rc->n_x, rc->n_y, renderdata, filename);
			}

			current_time_point = current_time_point + time_step;
			image_counter++;
		}
	}
	double Color_sequence_mean_time = timer.elapsed_time_milliseconds / nb_of_test_iterations;
	data_writer_ptr->writeToFile_endl(rendererID + suffix + "_seq_mean_time," + to_string(Color_sequence_mean_time));
}

void TestRunner::run_tests_sequence_optimized2(unique_ptr<DataWriter>& data_writer_ptr, LODTree4DRenderer_Interface* lodr_i, string rendererID_and_suffix, string output_data_filename_without_extension, double time_step)
{
	double frustrum_width = rc->frustrum->right - rc->frustrum->left;
	double pixel_diameter = frustrum_width / rc->n_x;
	double pixel_radius = 0.5 * pixel_diameter;
	double pixel_size = pixel_radius * pixel_radius; // * PI

	
	PerformanceTimer timer;
	string sequence_number_suffix;
	int nbOfDigitsInDecimal_gridsize = log10(tree4D->gridsize_T) + 1;
	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		cout << "  iteration " << to_string(i + 1) << " of " << to_string(nb_of_test_iterations) << endl;

		float current_time_point = 0.0;
		float image_counter = 0.0;
		while (current_time_point <= tree4D->gridsize_T) {

			showProgressBar(static_cast<int>(current_time_point), static_cast<int>(tree4D->gridsize_T), 10);
			// timed render
			memset(renderdata, 0, rc->n_x*rc->n_y * 4);
			timer.start();
			lodr_i->Render_optimized(*rc, tree4D, renderdata, current_time_point, pixel_size);
			timer.stop();

			if (i == nb_of_test_iterations - 1)
			{
				stringstream ss;
				ss << setw(nbOfDigitsInDecimal_gridsize) << setfill('0') << image_counter;
				sequence_number_suffix = ss.str();
				string filename = output_data_filename_without_extension + rendererID_and_suffix + "_seq" + sequence_number_suffix;
				writePPM(rc->n_x, rc->n_y, renderdata, filename);
			}

			current_time_point = current_time_point + time_step;
			image_counter++;
		}
	}
	double Color_sequence_mean_time = timer.elapsed_time_milliseconds / nb_of_test_iterations;
	data_writer_ptr->writeToFile_endl(rendererID_and_suffix + "_seq_mean_time," + to_string(Color_sequence_mean_time));
}
void TestRunner::run_tests_rotating_sphere_partial(
	unique_ptr<DataWriter>& data_writer_ptr, string suffix, vec3_d camera_eye,
	string output_data_filename_without_extension, double time_step_at_stack_distance, string expected_stacksize,
	LODWorkTree4DRenderer* lodwr, LODNormalTree4DRenderer* lodnr){

	camera->eye = camera_eye;
	data_writer_ptr->writeToFile_endl("camera eye: x: " + to_string(camera->eye[0]) + ", y: " + to_string(camera->eye[1]) + ", z: " + to_string(camera->eye[2]));

	//normals
	cout << "rotating Sphere: rendering normals sequence..." << endl;
	run_tests_sequence(data_writer_ptr, "normal", output_data_filename_without_extension + suffix, 1.0);
	data_writer_ptr->writeToFile_endl("");

	//work
	cout << "rotating Sphere: rendering work sequence..." << endl;
	run_tests_sequence(data_writer_ptr, "work", output_data_filename_without_extension + suffix, 1.0);
	data_writer_ptr->writeToFile_endl("");

	data_writer_ptr->writeToFile_endl("time_step," + to_string(time_step_at_stack_distance));
	//normals LOD
	cout << "rotating Sphere: rendering LOD normals sequence..." << endl;
	run_tests_sequence_optimized2(
		data_writer_ptr, lodnr, "LODNormal" + suffix,
		output_data_filename_without_extension, time_step_at_stack_distance);
	//get stack sizes
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	lodnr->Render(*rc, tree4D, renderdata, 0.0);
	data_writer_ptr->writeToFile_endl("LODNormal" + suffix + "_smallest_stack_size," + to_string(LODNormalTree4DRenderer::smallest_stack_size));
	data_writer_ptr->writeToFile_endl("LODNormal" + suffix + "_largest_stack_size," + to_string(LODNormalTree4DRenderer::largest_stack_size));
	data_writer_ptr->writeToFile_endl("expected stacksize,"+expected_stacksize);
	data_writer_ptr->writeToFile_endl("");


	//LOD work
	cout << "rotating Sphere: rendering LOD work sequence..." << endl;
	run_tests_sequence_optimized2(
		data_writer_ptr, lodwr, "LODWork" + suffix,
		output_data_filename_without_extension, time_step_at_stack_distance);
	//	//get stack sizes
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	lodwr->Render(*rc, tree4D, renderdata, 0.0);
	data_writer_ptr->writeToFile_endl("LODWork" + suffix + "_smallest_stack_size," + to_string(LODWorkTree4DRenderer::smallest_stack_size));
	data_writer_ptr->writeToFile_endl("LODWork" + suffix + "_largest_stack_size," + to_string(LODWorkTree4DRenderer::largest_stack_size));
	data_writer_ptr->writeToFile_endl("expected stacksize," + expected_stacksize);
	data_writer_ptr->writeToFile_endl("");

	data_writer_ptr->writeToFile_endl("---------");
	data_writer_ptr->writeToFile_endl("");
	
}