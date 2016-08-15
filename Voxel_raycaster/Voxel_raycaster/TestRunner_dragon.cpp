#include "TestRunner.h"
#include "Globals.h"
#include <iomanip>

void TestRunner::run_tests_dragon()
{


	string output_data_filename_without_extension = TestFilePaths::output_directory_path + "\\" + TestFilePaths::test_name_dragon;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_dragon, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();


	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer("LODWork"));
	if (lodwr != nullptr)
	{
		lodwr->max_level = camera_controller->level_to_render;
	}

	//get a high step count by already rendering it twice
	rmanager4D->setCurrentRenderer("LODWork");
	camera->eye = vec3_d(0.5, 0.5, 0.4);
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	rmanager4D->getCurrentRenderer()->Render(*rc, tree4D, renderdata, camera_controller->time_point);
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	rmanager4D->getCurrentRenderer()->Render(*rc, tree4D, renderdata, camera_controller->time_point);

	rmanager4D->setCurrentRenderer("Work");
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	rmanager4D->getCurrentRenderer()->Render(*rc, tree4D, renderdata, camera_controller->time_point);
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	rmanager4D->getCurrentRenderer()->Render(*rc, tree4D, renderdata, camera_controller->time_point);

	if(max_step_count< 405)
	{	
		max_step_count = 405;
	}

	
	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//=== WORK CLOSE :  LOD and NOT-LOD ===========================================//
	vec3_d camera_close = vec3_d(0.5, 0.5, -1.0);
	string suffix_close = "_close";
	run_tests_LOD_work_optimized(data_writer_ptr, camera_close, suffix_close, TestFilePaths::test_name_dragon);
	//run_tests_LOD_work(data_writer_ptr, camera_close, suffix_close, TestFilePaths::test_name_dragon);
	run_tests_work(data_writer_ptr, camera_close, suffix_close, TestFilePaths::test_name_dragon);

	data_writer_ptr->writeToFile_endl("");
	//=== WORK MEDIUM :  LOD and NOT-LOD ===========================================//
	vec3_d camera_medium = vec3_d(0.5, 0.5, -10.0);
	string suffix_medium = "_medium";
	run_tests_LOD_work_optimized(data_writer_ptr, camera_medium, suffix_medium, TestFilePaths::test_name_dragon);
	//run_tests_LOD_work(data_writer_ptr, camera_medium, suffix_medium, TestFilePaths::test_name_dragon);
	run_tests_work(data_writer_ptr, camera_medium, suffix_medium, TestFilePaths::test_name_dragon);
	
	data_writer_ptr->writeToFile_endl("");


	//=== WORK FAR :  LOD and NOT-LOD ===========================================//
	vec3_d camera_far = vec3_d(0.5, 0.5, -20.0);
	string suffix_far = "_far";
	run_tests_LOD_work_optimized(data_writer_ptr, camera_far, suffix_far, TestFilePaths::test_name_dragon);
	//run_tests_LOD_work(data_writer_ptr, camera_far, suffix_far, TestFilePaths::test_name_dragon);
	run_tests_work(data_writer_ptr, camera_far, suffix_far, TestFilePaths::test_name_dragon);
	
	
	delete tree4D;
	
}

void TestRunner::run_tests_translating_Suzanne()
{

	string output_data_filename_without_extension = TestFilePaths::output_directory_path + "\\" + TestFilePaths::test_name_translating_Suzanne;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_translating_Suzanne, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();


	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer("LODWork"));
	if (lodwr != nullptr)
	{
		lodwr->max_level = camera_controller->level_to_render;
	}
	camera->eye = vec3_d(-0.5, 0.5, 0.5);
	camera->setGaze( vec3_d(-1, 0, 0));
	camera->computeUVW();



	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	//time for image sequence:LOD
	//time for image sequence:NO LOD
	//time for image sequence: only changing images
	rmanager4D->setCurrentRenderer("color");

	run_tests_sequence(data_writer_ptr, "color", output_data_filename_without_extension, camera_controller->time_step);
	data_writer_ptr->writeToFile_endl("");

	//---------------------------------------------------------------------------------------------//
	rmanager4D->setCurrentRenderer("LODColor");
	run_tests_sequence_optimized(data_writer_ptr, "LODColor", "_timestep1",
		output_data_filename_without_extension, camera_controller->time_step);
	data_writer_ptr->writeToFile_endl("");

	//---------------------------------------------------------------------//
	run_tests_sequence_optimized(data_writer_ptr, "LODColor", "_timestep4",
		output_data_filename_without_extension, 4.0);
	data_writer_ptr->writeToFile_endl("");


	/*
	1.
	translating Suzanne
	S 2048, T 128
	max stack size = 12
	tree building - number of 8-element queues: 5
	tree building - number of 16-element queues: 7
	----------------------------------------------
	camera eye : xyx = (-0.120, 0.500, 0.475)
	camera gaxe: xyz = (-1.0, 0.0, 0.0)
	largest stack size: 10, smallest stack size: 10
	--> 10 - 5 = 5, 2^5 = 32 tijdsstappen worden weergegeven
	--> om de 4 tijdstappen
	-----------------------------------------------
	camera eye : xyx = (-1.120, 0.500, 0.475)
	camera gaxe: xyz = (-1.0, 0.0, 0.0)
	largest stack size: 9, smallest stack size: 9
	--> 9 - 5 = 4 , 2^4 = 16 tijdstappen worden weergegeven
	---------------------------------------------------


	WORK:
	Voor elke afstand:
	met LOD
	1. tijd om een sequentie te renderen
	2. tijd om het eerste image te renderen
	3. images
	4. hoe diep wordt er afgedaald

	zonder LOD
	1. tijd om een sequentie te renderen
	2. tijd om het eerste image te renderen
	3. images
	4. hoe diep wordt er afgedaald

	LEVEL: gewone level renderer op 1 tijdstip


	kunnen we meerdere modellen vergelijken?
	--> dan moet op dezelfde afstanden worden gerenderd
	*/




}




