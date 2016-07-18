#include "TriRenderer.h"
#include "omp.h"
#include "TriReader2.h"
#include "ShadeRec.h"
#include "Constants.h"
#include "../misc_math.h"


void TriRenderer::setTriReader(const std::string& filename, size_t n_triangles, size_t buffersize)
{
	reader = TriReader2(filename, n_triangles, buffersize);
}

TriRenderer::TriRenderer()
{
}

void TriRenderer::Render(const RenderContext& rc, unsigned char* texture_array) {

	//note: we gaan van een heel klein bestand uit!
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
	omp_set_num_threads(iCPU);
	// declare variables we use in loop
	int x, index, partindex;
	vec3_d to_light;
	double diffuse_factor, r, g, b, distancecut;
	
	
#pragma omp parallel for private(x,t,v,index,diffuse_factor,to_light,r,g,b)
	for (int y = 0; y < rc.n_y; y++) {
		partindex = y*(rc.n_y * 4);
		for (x = 0; x < rc.n_y; x++) {
			index = partindex + x * 4; // index in char array computation (part 2)

			vec3_d foundColor = vec3_d(0, 0, 0);
			ShadeRec sr = ShadeRec();
			reader.resetAfterLoop();

			double t_min = DOUBLE_MAX_VALUE;
			vec3_d normal = vec3_d();
			vec3_d hitPoint = vec3_d();

			Triangle triangle;
			Ray ray = rc.getRayForPixel(x, y);
			
			for (int current_tri = 0; current_tri < reader.n_triangles; current_tri++)
			{
				triangle = reader.getTriangle();
				bool hasHit =TriRenderer::hitTriangle(triangle, ray, sr);
				if(hasHit)
				{
					if(sr.t < t_min)
					{
						sr.hasHitAnObject = true;
						t_min = sr.t;
						normal = sr.normal;
						hitPoint = sr.hitPoint;
					}
				}
			}
			if(sr.hasHitAnObject)
			{
				double depth = len(rc.camera->eye - hitPoint);
				double grayValue = 1.0 / log(depth);
				foundColor = vec3_d(grayValue, grayValue, grayValue);
			}


			texture_array[index] = unsigned char(clampf(255 * foundColor[0], 0, 255));
			texture_array[index + 1] = unsigned char(clampf(255 * foundColor[1], 0, 255));
			texture_array[index + 2] = unsigned char(clampf(255 * foundColor[2], 0, 255));
			texture_array[index + 3] = unsigned char(1);


		}
	}

}

TriRenderer::~TriRenderer()
{
}

bool
TriRenderer::hitTriangle(const Triangle &triangle,const Ray& ray, ShadeRec& sr) {
	vec3_d v0(triangle.v0);
	vec3_d v1(triangle.v1);
	vec3_d v2(triangle.v2);

	double a = v0[0] - v1[0], b = v0[0] - v2[0], c = ray.direction[0], d = v0[0] - ray.origin[0];
	double e = v0[1] - v1[1], f = v0[1] - v2[1], g = ray.direction[1], h = v0[1] - ray.origin[1];
	double i = v0[2] - v1[2], j = v0[2] - v2[2], k = ray.direction[2], l = v0[2] - ray.origin[2];

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return (false);

	double r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0)
		return (false);

	if (beta + gamma > 1.0)
		return (false);

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < kEpsilon)
		return (false);

	sr.t = t;
	sr.normal = triangle.normal;  				// for flat shading
	sr.hitPoint = ray.origin + (t * ray.direction);

	return (true);
}

