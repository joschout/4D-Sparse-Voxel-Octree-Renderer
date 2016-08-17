#include "TestRunner.h"
#include "Globals.h"
#include <iomanip>
#include "Renderers/LODColorTree4DRenderer.h"
#include "Renderers/LODNormalTree4DRenderer.h"

void TestRunner::run_tests_dragon()
{
	cout << "Starting tests 'Stationary dragon... " << endl;

	string output_data_filename_without_extension 
		= TestFilePaths::output_directory_path 
		+ "\\" + TestFilePaths::test_name_dragon 
		+ "\\" + TestFilePaths::test_name_dragon;

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
	camera->setGaze(vec3_d(0, 0, -1));
	camera->computeUVW();

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
	cout << "rendering LOD work close..." << endl;

	vec3_d camera_close = vec3_d(0.5, 0.5, -1.0);
	string suffix_close = "_close";
	run_tests_LOD_work_optimized(data_writer_ptr, camera_close, suffix_close, output_data_filename_without_extension);
	//run_tests_LOD_work(data_writer_ptr, camera_close, suffix_close, TestFilePaths::test_name_dragon);
	cout << "rendering non-LOD work close..." << endl;
	run_tests_work(data_writer_ptr, camera_close, suffix_close, output_data_filename_without_extension);

	data_writer_ptr->writeToFile_endl("");
	//=== WORK MEDIUM :  LOD and NOT-LOD ===========================================//
	vec3_d camera_medium = vec3_d(0.5, 0.5, -10.0);
	string suffix_medium = "_medium";
	cout << "rendering LOD work medium..." << endl;
	run_tests_LOD_work_optimized(data_writer_ptr, camera_medium, suffix_medium, output_data_filename_without_extension);
	//run_tests_LOD_work(data_writer_ptr, camera_medium, suffix_medium, TestFilePaths::test_name_dragon);
	cout << "rendering non-LOD work medium..." << endl;
	run_tests_work(data_writer_ptr, camera_medium, suffix_medium, output_data_filename_without_extension);
	
	data_writer_ptr->writeToFile_endl("");


	//=== WORK FAR :  LOD and NOT-LOD ===========================================//
	vec3_d camera_far = vec3_d(0.5, 0.5, -20.0);
	string suffix_far = "_far";
	cout << "rendering LOD work far..." << endl;
	run_tests_LOD_work_optimized(data_writer_ptr, camera_far, suffix_far, output_data_filename_without_extension);
	//run_tests_LOD_work(data_writer_ptr, camera_far, suffix_far, TestFilePaths::test_name_dragon);
	cout << "rendering non-LOD work far..." << endl;
	run_tests_work(data_writer_ptr, camera_far, suffix_far, output_data_filename_without_extension);
	
	
	delete tree4D;
	
}

void TestRunner::run_tests_translating_Suzanne()
{

	cout << "Starting tests 'Translating Suzanne... " << endl;

	string output_data_filename_without_extension 
		= TestFilePaths::output_directory_path + "\\" 
		+ TestFilePaths::test_name_translating_Suzanne;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_translating_Suzanne, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();


	LODColorTree4DRenderer* lodcr = dynamic_cast<LODColorTree4DRenderer*>(rmanager4D->getRenderer("LODColor"));
	if (lodcr != nullptr)
	{
		lodcr->max_level = camera_controller->level_to_render;
	}
	else
	{
		cout << "lodcr is a NULLPOINTER" << endl;
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
	cout << "translating Suzanne: rendering color sequence..." << endl;
	run_tests_sequence(data_writer_ptr, "color", output_data_filename_without_extension, camera_controller->time_step);
	data_writer_ptr->writeToFile_endl("");

	//---------------------------------------------------------------------------------------------//
	cout << "translating Suzanne: rendering LOD-color sequence with normal timestep..." << endl;

	string rendererID_LODColor = "LODColor";
	string suffix = "_timestep1";
	double time_step = camera_controller->time_step;
	rmanager4D->setCurrentRenderer(rendererID_LODColor);

	run_tests_sequence_optimized(data_writer_ptr, rendererID_LODColor, suffix,
		output_data_filename_without_extension, time_step);

	// get the stack sizes
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	lodcr->Render(*rc, tree4D, renderdata, 0.0);
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix +"_smallest_stack_size," + to_string(LODColorTree4DRenderer::smallest_stack_size));
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_largest_stack_size," + to_string(LODColorTree4DRenderer::largest_stack_size));
	data_writer_ptr->writeToFile_endl("");

	//---------------------------------------------------------------------//
	suffix = "_timestep4";
	time_step = 4.0;
	cout << "translating Suzanne: rendering LOD-color sequence with timestep "+ to_string(time_step) + " ..." << endl;

	run_tests_sequence_optimized(data_writer_ptr, rendererID_LODColor, suffix,
		output_data_filename_without_extension, time_step);

	// get the stack sizes
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	lodcr->Render(*rc, tree4D, renderdata, 0.0);
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_smallest_stack_size," + to_string(LODColorTree4DRenderer::smallest_stack_size));
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_largest_stack_size," + to_string(LODColorTree4DRenderer::largest_stack_size));
	data_writer_ptr->writeToFile_endl("");

	//---------------------------------------------------------------------//
	lodcr->max_level = 7.0;
	suffix = "_timestep32";
	time_step = 32.0;
	cout << "translating Suzanne: rendering LOD-color sequence with timestep " + to_string(time_step) + " ..." << endl;

	data_writer_ptr->writeToFile_endl("max_level," + to_string(lodcr->max_level));
	run_tests_sequence_optimized(data_writer_ptr, rendererID_LODColor, suffix,
		output_data_filename_without_extension, time_step);

	// get the stack sizes
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	lodcr->Render(*rc, tree4D, renderdata, 0.0);
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_smallest_stack_size," + to_string(LODColorTree4DRenderer::smallest_stack_size));
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_largest_stack_size," + to_string(LODColorTree4DRenderer::largest_stack_size));
	data_writer_ptr->writeToFile_endl("");
	
	//---------------------------------------------------------------------//
	lodcr->max_level = 6.0;
	suffix = "_timestep64";
	time_step = 64.0;
	cout << "translating Suzanne: rendering LOD-color sequence with timestep " + to_string(time_step) + " ..." << endl;
	data_writer_ptr->writeToFile_endl("max_level," + to_string(lodcr->max_level));
	run_tests_sequence_optimized(data_writer_ptr, rendererID_LODColor, suffix,
		output_data_filename_without_extension, time_step);

	// get the stack sizes
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	lodcr->Render(*rc, tree4D, renderdata, 0.0);
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_smallest_stack_size," + to_string(LODColorTree4DRenderer::smallest_stack_size));
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_largest_stack_size," + to_string(LODColorTree4DRenderer::largest_stack_size));
	data_writer_ptr->writeToFile_endl("");
	
	//---------------------------------------------------------------------//
	lodcr->max_level = 5;
	suffix = "_timestep128";
	time_step = 128.0;
	cout << "translating Suzanne: rendering LOD-color sequence with timestep " + to_string(time_step) + " ..." << endl;
	data_writer_ptr->writeToFile_endl("max_level," + to_string(lodcr->max_level));
	run_tests_sequence_optimized(data_writer_ptr, rendererID_LODColor, suffix,
		output_data_filename_without_extension, time_step);

	// get the stack sizes
	memset(renderdata, 0, rc->n_x*rc->n_y * 4);
	lodcr->Render(*rc, tree4D, renderdata, 0.0);
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_smallest_stack_size," + to_string(LODColorTree4DRenderer::smallest_stack_size));
	data_writer_ptr->writeToFile_endl(rendererID_LODColor + suffix + "_largest_stack_size," + to_string(LODColorTree4DRenderer::largest_stack_size));
	data_writer_ptr->writeToFile_endl("");


	delete tree4D;
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
	--> 12 - 10 = 2, 2^2 = 4 tijdstappen worden weergegeven
	--> om de 4 tijdstappen
	-----------------------------------------------
	camera eye : xyx = (-1.120, 0.500, 0.475)
	camera gaxe: xyz = (-1.0, 0.0, 0.0)
	largest stack size: 9, smallest stack size: 9
	--> 12 - 9 = 3, 2^3 = 8 tijdstappen worden weergegeven
	--> om de 8 tijdstappen
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

void TestRunner::run_tests_sphere_rotating()
{

	cout << "Starting tests 'Rotating sphere... " << endl;

	string output_data_filename_without_extension
		= TestFilePaths::output_directory_path + "\\"
		+ TestFilePaths::test_name_sphere_rotating + "\\"
		+ TestFilePaths::test_name_sphere_rotating;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_sphere_rotating, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();


	camera->eye = vec3_d(-0.5, 0.5, 0.5);
	camera->setGaze(vec3_d(-1, 0, 0));
	camera->computeUVW();

	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	string rendererID_work = "work";
	string rendererID_normal = "normal";
	string rendererID_LOD_normal = "LODNormal";
	string rendererID_LOD_work = "LODWork";


	//IMPORTANT
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_normal));
	if(lodnr == nullptr)
	{
		cout << "lodnormal_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}else
	{
		lodnr->max_level = camera_controller->level_to_render;
	}

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_work));
	if (lodwr == nullptr)
	{
		cout << "lodwork_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}else
	{
		lodwr->max_level = camera_controller->level_to_render;
	}


	// ----------------------------------------------------------//
	double time_step_stack_11 = 2.0;
	string suffix = "_distance_stack_11";
	string expected_stacksize = "11";
	vec3_d camera_eye_stack_11 = vec3_d(-0.1, 0.5, 0.5);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_11, output_data_filename_without_extension,
		time_step_stack_11, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_10 = 4.0;
	suffix = "_distance_stack_10";
	expected_stacksize = "10";
	vec3_d camera_eye_stack_10 = vec3_d(-1.0, 0.5, 0.5);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_10, output_data_filename_without_extension,
		time_step_stack_10, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_9 = 8.0;
	suffix = "_distance_stack_9";
	 expected_stacksize = "9";
	vec3_d camera_eye_stack_9 = vec3_d(-4.5, 0.5, 0.5);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_9, output_data_filename_without_extension,
		time_step_stack_9, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_8 = 16.0;
	suffix = "_distance_stack_8_close";
	vec3_d camera_eye_stack_8_close = vec3_d(-17.5, 0.5, 0.5);
	expected_stacksize = "8";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8_close, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//

	suffix = "_distance_stack_8_far";//"_timestep16_far";
	vec3_d camera_eye_stack_8_far = vec3_d(-70.0, 0.5, 0.5);
	expected_stacksize = "8";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8_far, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);
	

	delete tree4D;
}

void TestRunner::run_tests_exploding_dragon()
{
	cout << "Starting tests 'Exploding dragon... " << endl;

	string output_data_filename_without_extension
		= TestFilePaths::output_directory_path + "\\"
		+ TestFilePaths::test_name_exploding_dragon + "\\"
		+ TestFilePaths::test_name_exploding_dragon;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_exploding_dragon, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();

	camera->eye = vec3_d(-0.5, 0.5, 0.5);
	camera->setGaze(vec3_d(-1, 0, 0));
	camera->computeUVW();


	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	string rendererID_work = "work";
	string rendererID_normal = "normal";
	string rendererID_LOD_normal = "LODNormal";
	string rendererID_LOD_work = "LODWork";


	//IMPORTANT
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_normal));
	if (lodnr == nullptr)
	{
		cout << "lodnormal_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodnr->max_level = camera_controller->level_to_render;
	}

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_work));
	if (lodwr == nullptr)
	{
		cout << "lodwork_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodwr->max_level = camera_controller->level_to_render;
	}


	// ----------------------------------------------------------//
	double time_step_stack_11 = 2.0;
	string suffix = "_distance_stack_11";
	string expected_stacksize = "11";
	vec3_d camera_eye_stack_11 = vec3_d(-0.1, 0.5, 0.5);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_11, output_data_filename_without_extension,
		time_step_stack_11, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_10 = 4.0;
	suffix = "_distance_stack_10";
	expected_stacksize = "10";
	vec3_d camera_eye_stack_10 = vec3_d(-1.0, 0.5, 0.5);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_10, output_data_filename_without_extension,
		time_step_stack_10, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_9 = 8.0;
	suffix = "_distance_stack_9";
	expected_stacksize = "9";
	vec3_d camera_eye_stack_9 = vec3_d(-4.5, 0.5, 0.5);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_9, output_data_filename_without_extension,
		time_step_stack_9, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_8 = 16.0;
	suffix = "_distance_stack_8_close";
	vec3_d camera_eye_stack_8_close = vec3_d(-17.5, 0.5, 0.5);
	expected_stacksize = "8";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8_close, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//

	suffix = "_distance_stack_8_far";//"_timestep16_far";
	vec3_d camera_eye_stack_8_far = vec3_d(-70.0, 0.5, 0.5);
	expected_stacksize = "8";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8_far, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);



	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	delete tree4D;
}

void TestRunner::run_tests_AEK_24_cell()
{
	cout << "Starting tests 'AEK cell... " << endl;

	string output_data_filename_without_extension
		= TestFilePaths::output_directory_path + "\\"
		+ TestFilePaths::test_name_AEK_24_cell + "\\"
		+ TestFilePaths::test_name_AEK_24_cell;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_AEK_24_cell, tree4D);

	tree4D->min = vec4_d(0.0, 0.0, 0.25, 0.0);
	tree4D->max = vec4_d(0.25, 0.25, 0.0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();

	camera->eye = vec3_d(-0.5, 0.125, 0.125);
	camera->setGaze(vec3_d(-1, 0, 0));
	camera->computeUVW();




	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	string rendererID_work = "work";
	string rendererID_normal = "normal";
	string rendererID_LOD_normal = "LODNormal";
	string rendererID_LOD_work = "LODWork";


	//IMPORTANT
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_normal));
	if (lodnr == nullptr)
	{
		cout << "lodnormal_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodnr->max_level = camera_controller->level_to_render;
	}

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_work));
	if (lodwr == nullptr)
	{
		cout << "lodwork_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodwr->max_level = camera_controller->level_to_render;
	}


	// ----------------------------------------------------------//
	double time_step_stack_10 = 1.0;
	string suffix = "_distance_stack_10";
	string expected_stacksize = "10";
	vec3_d camera_eye_stack_10 = vec3_d(-0.1, 0.125, 0.125);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_10, output_data_filename_without_extension,
		time_step_stack_10, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_9 = 2.0;
	suffix = "_distance_stack_9";
	expected_stacksize = "9";
	vec3_d camera_eye_stack_9 = vec3_d(-0.3, 0.125, 0.125);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_9, output_data_filename_without_extension,
		time_step_stack_9, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_8 = 4.0;
	suffix = "_distance_stack_8";
	expected_stacksize = "8";
	vec3_d camera_eye_stack_8 = vec3_d(-1.2, 0.125, 0.125);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_7 = 8.0;
	suffix = "_distance_stack_7";
	vec3_d camera_eye_stack_7 = vec3_d(-4.5, 0.125, 0.125);
	expected_stacksize = "7";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_7, output_data_filename_without_extension,
		time_step_stack_7, expected_stacksize, lodwr, lodnr);


	delete tree4D;
}

void TestRunner::run_tests_cloth()
{
	cout << "Starting tests cloth... " << endl;

	string output_data_filename_without_extension
		= TestFilePaths::output_directory_path + "\\"
		+ TestFilePaths::test_name_cloth + "\\"
		+ TestFilePaths::test_name_cloth;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_cloth, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();

	camera->eye = vec3_d(0.185, 0.125, 0.028);
	camera->setGaze(vec3_d(0.342, 0.0, -0.940));
	camera->computeUVW();




	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	string rendererID_work = "work";
	string rendererID_normal = "normal";
	string rendererID_LOD_normal = "LODNormal";
	string rendererID_LOD_work = "LODWork";


	//IMPORTANT
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_normal));
	if (lodnr == nullptr)
	{
		cout << "lodnormal_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodnr->max_level = camera_controller->level_to_render;
	}

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_work));
	if (lodwr == nullptr)
	{
		cout << "lodwork_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodwr->max_level = camera_controller->level_to_render;
	}


	// ----------------------------------------------------------//
	double time_step_stack_10 = 1.0;
	string suffix = "_distance_stack_10";
	string expected_stacksize = "10";
	vec3_d camera_eye_stack_10 = vec3_d(0.185, 0.125, 0.028);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_10, output_data_filename_without_extension,
		time_step_stack_10, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_9 = 2.0;
	suffix = "_distance_stack_9";
	expected_stacksize = "9";
	vec3_d camera_eye_stack_9 = vec3_d(0.322, 0.125, -0.348);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_9, output_data_filename_without_extension,
		time_step_stack_9, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_8 = 4.0;
	suffix = "_distance_stack_8";
	expected_stacksize = "8";
	vec3_d camera_eye_stack_8 = vec3_d(0.595, 0.125, -1.099);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_7 = 8.0;
	suffix = "_distance_stack_7";
	vec3_d camera_eye_stack_7 = vec3_d(1.724, 0.125, -4.2);
	expected_stacksize = "7";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_7, output_data_filename_without_extension,
		time_step_stack_7, expected_stacksize, lodwr, lodnr);

	delete tree4D;
}

void TestRunner::run_tests_fairy_forest()
{
	cout << "Starting tests 'Fairy forest... " << endl;

	string output_data_filename_without_extension
		= TestFilePaths::output_directory_path + "\\"
		+ TestFilePaths::test_name_fairy_forest + "\\"
		+ TestFilePaths::test_name_fairy_forest;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_fairy_forest, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();

	camera->eye = vec3_d(0.125, 0.248, 0.018);
	camera->setGaze(vec3_d(0.0, 0.5, -0.866));
	camera->computeUVW();




	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	string rendererID_work = "work";
	string rendererID_normal = "normal";
	string rendererID_LOD_normal = "LODNormal";
	string rendererID_LOD_work = "LODWork";


	//IMPORTANT
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_normal));
	if (lodnr == nullptr)
	{
		cout << "lodnormal_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodnr->max_level = camera_controller->level_to_render;
	}

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_work));
	if (lodwr == nullptr)
	{
		cout << "lodwork_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodwr->max_level = camera_controller->level_to_render;
	}


	// ----------------------------------------------------------//
	double time_step_stack_10 = 1.0;
	string suffix = "_distance_stack_10";
	string expected_stacksize = "10";
	vec3_d camera_eye_stack_10 = vec3_d(0.125, 0.195, 0.79);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_10, output_data_filename_without_extension,
		time_step_stack_10, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_9 = 2.0;
	suffix = "_distance_stack_9";
	expected_stacksize = "9";
	vec3_d camera_eye_stack_9 = vec3_d(0.125, 0.395, -0.267);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_9, output_data_filename_without_extension,
		time_step_stack_9, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_8 = 4.0;
	suffix = "_distance_stack_8";
	expected_stacksize = "8";
	vec3_d camera_eye_stack_8 = vec3_d(0.125, 0.795, -0.960);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_7 = 8.0;
	suffix = "_distance_stack_7";
	vec3_d camera_eye_stack_7 = vec3_d(0.125, 2.445, -3.818);
	expected_stacksize = "7";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_7, output_data_filename_without_extension,
		time_step_stack_7, expected_stacksize, lodwr, lodnr);

	delete tree4D;
}

void TestRunner::run_tests_flag()
{
	cout << "Starting tests flag... " << endl;

	string output_data_filename_without_extension
		= TestFilePaths::output_directory_path + "\\"
		+ TestFilePaths::test_name_flag + "\\"
		+ TestFilePaths::test_name_flag;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_flag, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();

	camera->eye = vec3_d(-0.5, 0.5, 0.5);
	camera->setGaze(vec3_d(-0.766, 0.0, -0.643));
	camera->computeUVW();

	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	string rendererID_work = "work";
	string rendererID_normal = "normal";
	string rendererID_LOD_normal = "LODNormal";
	string rendererID_LOD_work = "LODWork";


	//IMPORTANT
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_normal));
	if (lodnr == nullptr)
	{
		cout << "lodnormal_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodnr->max_level = camera_controller->level_to_render;
	}

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_work));
	if (lodwr == nullptr)
	{
		cout << "lodwork_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodwr->max_level = camera_controller->level_to_render;
	}



	// ----------------------------------------------------------//
	double time_step_stack_10 = 1.0;
	string suffix = "_distance_stack_10";
	string expected_stacksize = "10";
	vec3_d camera_eye_stack_10 = vec3_d(0.005, 0.125, 0.036);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_10, output_data_filename_without_extension,
		time_step_stack_10, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_9 = 2.0;
	suffix = "_distance_stack_9";
	expected_stacksize = "9";
	vec3_d camera_eye_stack_9 = vec3_d(-0.148, 0.125, -0.092);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_9, output_data_filename_without_extension,
		time_step_stack_9, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_8 = 4.0;
	suffix = "_distance_stack_8";
	expected_stacksize = "8";
	vec3_d camera_eye_stack_8 = vec3_d(-0.761, 0.125, -0.607);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_7 = 8.0;
	suffix = "_distance_stack_7";
	vec3_d camera_eye_stack_7 = vec3_d(-3.365, 0.125, -2.792);
	expected_stacksize = "7";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_7, output_data_filename_without_extension,
		time_step_stack_7, expected_stacksize, lodwr, lodnr);

	delete tree4D;
}

void TestRunner::run_tests_marbles()
{
	cout << "Starting tests marbles... " << endl;

	string output_data_filename_without_extension
		= TestFilePaths::output_directory_path + "\\"
		+ TestFilePaths::test_name_marbles + "\\"
		+ TestFilePaths::test_name_marbles;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_marbles, tree4D);

	tree4D->min = vec4_d(0.0, 0.0, 0.25, 0.0);
	tree4D->max = vec4_d(0.25, 0.25, 0.0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();

	camera->eye = vec3_d(-0.5, 0.125, 0.125);
	camera->setGaze(vec3_d(-1, 0, 0));
	camera->computeUVW();



	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	string rendererID_work = "work";
	string rendererID_normal = "normal";
	string rendererID_LOD_normal = "LODNormal";
	string rendererID_LOD_work = "LODWork";


	//IMPORTANT
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_normal));
	if (lodnr == nullptr)
	{
		cout << "lodnormal_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodnr->max_level = camera_controller->level_to_render;
	}

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_work));
	if (lodwr == nullptr)
	{
		cout << "lodwork_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodwr->max_level = camera_controller->level_to_render;
	}


	// ----------------------------------------------------------//
	double time_step_stack_10 = 1.0;
	string suffix = "_distance_stack_10";
	string expected_stacksize = "10";
	vec3_d camera_eye_stack_10 = vec3_d(-0.1, 0.125, 0.125);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_10, output_data_filename_without_extension,
		time_step_stack_10, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_9 = 2.0;
	suffix = "_distance_stack_9";
	expected_stacksize = "9";
	vec3_d camera_eye_stack_9 = vec3_d(-0.3, 0.125, 0.125);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_9, output_data_filename_without_extension,
		time_step_stack_9, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_8 = 4.0;
	suffix = "_distance_stack_8";
	expected_stacksize = "8";
	vec3_d camera_eye_stack_8 = vec3_d(-1.2, 0.125, 0.125);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_7 = 8.0;
	suffix = "_distance_stack_7";
	vec3_d camera_eye_stack_7 = vec3_d(-4.5, 0.125, 0.125);
	expected_stacksize = "7";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_7, output_data_filename_without_extension,
		time_step_stack_7, expected_stacksize, lodwr, lodnr);


	delete tree4D;
}

void TestRunner::run_tests_sintel_walk_cycle()
{
	cout << "Starting tests Sintel walk cycle... " << endl;

	string output_data_filename_without_extension
		= TestFilePaths::output_directory_path + "\\"
		+ TestFilePaths::test_name_sintel_walk_cycle + "\\"
		+ TestFilePaths::test_name_sintel_walk_cycle;
	unique_ptr<DataWriter> data_writer_ptr(new DataWriter(output_data_filename_without_extension));

	readTree4D(TestFilePaths::filename_sintel_walk_cycle, tree4D);

	tree4D->min = vec4_d(0, 0, 1, 0);
	tree4D->max = vec4_d(1, 1, 0, tree4D->gridsize_T);
	*camera_controller = CameraController(camera, inputformat, nullptr, rmanager4D, tree4D, rc, renderdata);

	size_t max_stack_size = log(max(tree4D->gridsize_S, tree4D->gridsize_T)) / log(2) + 1;
	camera_controller->level_to_render = max_stack_size;
	camera_controller->time_step = abs(tree4D->min[3] - tree4D->max[3]) / tree4D->gridsize_T;
	camera_controller->moveCamera();

	camera->eye = vec3_d(0.125, 0.125, -1.0);
	camera->setGaze(vec3_d(0, 0, -1.0));
	camera->computeUVW();




	data_writer_ptr->writeToFile_endl("gridsize_S," + to_string(tree4D->gridsize_S));
	data_writer_ptr->writeToFile_endl("gridsize_t," + to_string(tree4D->gridsize_T));
	data_writer_ptr->writeToFile_endl("n_nodes," + to_string(tree4D->n_nodes));

	data_writer_ptr->writeToFile_endl("max_stack_size," + to_string(max_stack_size));
	data_writer_ptr->writeToFile_endl("");
	//-----------------------------------------------------------------------------------//

	string rendererID_work = "work";
	string rendererID_normal = "normal";
	string rendererID_LOD_normal = "LODNormal";
	string rendererID_LOD_work = "LODWork";


	//IMPORTANT
	LODNormalTree4DRenderer* lodnr = dynamic_cast<LODNormalTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_normal));
	if (lodnr == nullptr)
	{
		cout << "lodnormal_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodnr->max_level = camera_controller->level_to_render;
	}

	LODWorkTree4DRenderer* lodwr = dynamic_cast<LODWorkTree4DRenderer*>(rmanager4D->getRenderer(rendererID_LOD_work));
	if (lodwr == nullptr)
	{
		cout << "lodwork_r is a NULLPOINTER" << endl;
		std::cout << "Press ENTER to continue...";
		cin.get();
	}
	else
	{
		lodwr->max_level = camera_controller->level_to_render;
	}


	// ----------------------------------------------------------//
	double time_step_stack_10 = 1.0;
	string suffix = "_distance_stack_10";
	string expected_stacksize = "10";
	vec3_d camera_eye_stack_10 = vec3_d(0.125, 0.125, -0.10);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_10, output_data_filename_without_extension,
		time_step_stack_10, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_9 = 2.0;
	suffix = "_distance_stack_9";
	expected_stacksize = "9";
	vec3_d camera_eye_stack_9 = vec3_d(0.125, 0.125, -0.3);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_9, output_data_filename_without_extension,
		time_step_stack_9, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_8 = 4.0;
	suffix = "_distance_stack_8";
	expected_stacksize = "8";
	vec3_d camera_eye_stack_8 = vec3_d(0.125, 0.125, -1.2);
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_8, output_data_filename_without_extension,
		time_step_stack_8, expected_stacksize, lodwr, lodnr);

	//--------------------------------------------------------//
	double time_step_stack_7 = 8.0;
	suffix = "_distance_stack_7";
	vec3_d camera_eye_stack_7 = vec3_d(0.125, 0.125, -4.5);
	expected_stacksize = "7";
	run_tests_rotating_sphere_partial(
		data_writer_ptr, suffix,
		camera_eye_stack_7, output_data_filename_without_extension,
		time_step_stack_7, expected_stacksize, lodwr, lodnr);



	delete tree4D;
}
