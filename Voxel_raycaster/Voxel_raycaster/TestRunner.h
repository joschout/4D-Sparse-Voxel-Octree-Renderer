#ifndef TESTRUNNER_H
#define TESTRUNNER_H
#include "Tree4DReader.h"
#include "CameraControl.h"
#include "Renderers/LODWorkTree4DRenderer.h"
#include "timer.h"
#include "DataWriter.h"
#include "Tests_file_paths.h"


class TestRunner
{
public:
	Tree4D* tree4D;
	Camera* camera;
	CameraController* camera_controller;
	FileFormat * inputformat;
	RendererManager4D * rmanager4D;
	RenderContext *rc;
	unsigned char* renderdata;


	int nb_of_test_iterations;

	TestRunner(Camera* camera, CameraController* camera_controller, FileFormat *inputformat, RendererManager4D *render_manager4D, Tree4D * tree4D, RenderContext *rc, unsigned char* renderdata);

	void run_tests_dragon();

	void run_tests_translating_Suzanne();

	double render_nb_of_iterations() const;
	void run_tests_LOD_work(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string test_name);
	void run_tests_LOD_work_optimized(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string test_name);
	void run_tests_work(unique_ptr<DataWriter>& data_writer_ptr, vec3_d camera_eye, string suffix, string test_name);

	void run_tests_sequence(
		unique_ptr<DataWriter>& data_writer_ptr, string rendererID,
		string output_data_filename_without_extension, double time_step);
	void run_tests_sequence_optimized(
		unique_ptr<DataWriter>& data_writer_ptr, string rendererID, string suffix,
		string output_data_filename_without_extension, double time_step);
};



#endif