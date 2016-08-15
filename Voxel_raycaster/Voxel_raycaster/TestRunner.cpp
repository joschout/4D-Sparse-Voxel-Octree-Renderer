#include "TestRunner.h"
#include <iomanip>
#include "Renderers/LODColorTree4DRenderer.h"


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

void TestRunner::run_tests_LOD_work(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string test_name)
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
	string filename_image = TestFilePaths::output_directory_path + "\\" + test_name + "_LODWork" + suffix;
	writePPM(rc->n_x, rc->n_y, renderdata, filename_image);
}

void TestRunner::run_tests_LOD_work_optimized(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string test_name)
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
	string filename_image = TestFilePaths::output_directory_path + "\\" + test_name + "_LODWork" + suffix;
	writePPM(rc->n_x, rc->n_y, renderdata, filename_image);
}

void TestRunner::run_tests_work(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string test_name)
{

	camera_eye = camera_eye;

	rmanager4D->setCurrentRenderer("Work");
	//render once to get max amount of work in steps
	double work_far_mean_time = render_nb_of_iterations();
	data_writer_ptr->writeToFile_endl("work_mean_time" + suffix + "," + to_string(work_far_mean_time));


	// write image to disk
	string filename_image = TestFilePaths::output_directory_path + "\\" + test_name + "_Work" + suffix;
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
		for (int time_i = 0; time_i <= tree4D->gridsize_T; time_i++) {

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

	LODTree4DRenderer_Interface* lodr_interface = dynamic_cast<LODTree4DRenderer_Interface*>(rmanager4D->getRenderer(rendererID));


	PerformanceTimer timer;
	string sequence_number_suffix;
	int nbOfDigitsInDecimal_gridsize = log10(tree4D->gridsize_T) + 1;
	for (int i = 0; i < nb_of_test_iterations; i++)
	{


		float current_time_point = 0.0;
		float image_counter = 0.0;
		while (current_time_point <= tree4D->gridsize_T){

			// timed render
			memset(renderdata, 0, rc->n_x*rc->n_y * 4);
			timer.start();
			lodr_interface->Render_optimized(*rc, tree4D, renderdata, current_time_point, pixel_size);
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