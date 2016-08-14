#ifndef TESTRUNNER_H
#define TESTRUNNER_H
#include "Tree4DReader.h"
#include "CameraControl.h"
#include "Renderers/LODWorkTree4DRenderer.h"
#include "timer.h"
#include "DataWriter.h"

void inline run_tests_dragon(Tree4D* tree4D, unsigned char* renderdata, RendererManager4D rmanager4D, CameraController& camera_controller, Camera& camera, RenderContext& render_context)
{
	std::string filename_dragon = "D:\\Masterproef\\Modellen\\dragon\\tree4d\\dragon_S4096_T1_P256_color_from_model.tree4d";
	//std::string filename_dragon = "D:\\Masterproef\\Modellen\\dragon\\tree4d\\dragon_S2048_T1_P16_linear_LEVELS.tree4d";
	std::string test_name = "dragon_S2048_T1_P16_linear_LEVELS";
	std::string output_directory_path = "D:\\Masterproef\\Voxel_raytracer_test_output";

	string output_data_filename_without_extension = output_directory_path + "\\" + test_name;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));
	
	PerformanceTimer LOD_work_close_timer = PerformanceTimer();
	PerformanceTimer work_close_timer = PerformanceTimer();
	PerformanceTimer LOD_work_far_timer = PerformanceTimer();
	PerformanceTimer work_far_timer = PerformanceTimer();

	readTree4D(filename_dragon, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	FileFormat inputformat = TREE4D;
	camera_controller = CameraController(&camera, &inputformat, nullptr, &rmanager4D, tree4D, &render_context, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller.level_to_render = max_stack_size;
	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	camera_controller.time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller.moveCamera();


	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D.getRenderer("LODWork"));
	if (lodwr != nullptr)
	{
		lodwr->max_level = camera_controller.level_to_render;
	}
	
	int nb_of_test_iterations = 10;

	//=== WORK CLOSE :  LOD and NOT-LOD ===========================================//


	data_writer_ptr->writeToFile_endl("Work close");
	camera.eye = vec3_d(0.5, 0.5, -0.2);

	rmanager4D.setCurrentRenderer("LODWork");
	//render once to get max amount of work in steps
	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
	lodwr->Render(render_context, tree4D, renderdata, camera_controller.time_point);

	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		//timed render
		memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
		LOD_work_close_timer.start();
		lodwr->Render(render_context, tree4D, renderdata, camera_controller.time_point);
		LOD_work_close_timer.stop();

	}
	double LOD_work_close_mean_time = LOD_work_close_timer.elapsed_time_milliseconds / nb_of_test_iterations;
	size_t LOD_work_close_smallest_stack_size = LODWorkTree4DRenderer::smallest_stack_size;
	size_t LOD_work_close_largest_stack_size = LODWorkTree4DRenderer::largest_stack_size;
	data_writer_ptr->writeToFile_endl("LOD_work_close_mean_time," + to_string(LOD_work_close_mean_time));
	data_writer_ptr->writeToFile_endl("LOD_work_close_smallest_stack_size," + to_string(LOD_work_close_smallest_stack_size));
	data_writer_ptr->writeToFile_endl("LOD_work_close_largest_stack_size," + to_string(LOD_work_close_largest_stack_size));

	
	//write image to disk
	std::string filename_image = output_directory_path + "\\" + test_name + "_LODWork_close";
	writePPM(render_context.n_x, render_context.n_y, renderdata, filename_image);
	//----------------------------------------------------------------------------//
	data_writer_ptr->writeToFile_endl("Work far");


	rmanager4D.setCurrentRenderer("Work");
	//render once to get max amount of work in steps
	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
	rmanager4D.getCurrentRenderer()->Render(render_context, tree4D, renderdata, camera_controller.time_point);
	
	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		// timed render
		memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
		work_close_timer.start();
		rmanager4D.getCurrentRenderer()->Render(render_context, tree4D, renderdata, camera_controller.time_point);
		work_close_timer.stop();
	}
	double work_close_mean_time = work_close_timer.elapsed_time_milliseconds / nb_of_test_iterations;
	data_writer_ptr->writeToFile_endl("work_close_mean_time," + to_string(work_close_mean_time));

	// write image to disk
	filename_image = output_directory_path + "\\" + test_name + "_Work_close";
	writePPM(render_context.n_x, render_context.n_y, renderdata, filename_image);


	//=== WORK FAR :  LOD and NOT-LOD ===========================================//
	camera.eye = vec3_d(0.5, 0.5, -10.0);

	rmanager4D.setCurrentRenderer("LODWork");
	//render once to get max amount of work in steps
	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
	lodwr->Render(render_context, tree4D, renderdata, camera_controller.time_point);

	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		//timed render
		memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
		LOD_work_far_timer.start();
		lodwr->Render(render_context, tree4D, renderdata, camera_controller.time_point);
		LOD_work_far_timer.stop();

	}
	double LOD_work_far_mean_time = LOD_work_far_timer.elapsed_time_milliseconds / nb_of_test_iterations;
	size_t LOD_work_far_smallest_stack_size = LODWorkTree4DRenderer::smallest_stack_size;
	size_t LOD_work_far_largest_stack_size = LODWorkTree4DRenderer::largest_stack_size;
	data_writer_ptr->writeToFile_endl("LOD_work_far_mean_time," + to_string(LOD_work_far_mean_time));
	data_writer_ptr->writeToFile_endl("LOD_work_far_smallest_stack_size," + to_string(LOD_work_far_smallest_stack_size));
	data_writer_ptr->writeToFile_endl("LOD_work_far_largest_stack_size," + to_string(LOD_work_far_largest_stack_size));


	//write image to disk
	filename_image = output_directory_path + "\\" + test_name + "_LODWork_far";
	writePPM(render_context.n_x, render_context.n_y, renderdata, filename_image);
	//----------------------------------------------------------------------------//

	rmanager4D.setCurrentRenderer("Work");
	//render once to get max amount of work in steps
	memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
	rmanager4D.getCurrentRenderer()->Render(render_context, tree4D, renderdata, camera_controller.time_point);
	for (int i = 0; i < nb_of_test_iterations; i++)
	{
		// timed render
		memset(renderdata, 0, render_context.n_x*render_context.n_y * 4);
		work_far_timer.start();
		rmanager4D.getCurrentRenderer()->Render(render_context, tree4D, renderdata, camera_controller.time_point);
		work_far_timer.stop();
	}
	double work_far_mean_time = work_far_timer.elapsed_time_milliseconds / nb_of_test_iterations;
	data_writer_ptr->writeToFile_endl("work_far_mean_time," + to_string(work_far_mean_time));


	// write image to disk
	filename_image = output_directory_path + "\\" + test_name + "_Work_far";
	writePPM(render_context.n_x, render_context.n_y, renderdata, filename_image);





}


#endif