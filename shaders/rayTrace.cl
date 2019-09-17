#include "shaders/Noise2.cl"
#define MAX_MARCHING_STEPS 1000

__constant float EPSILON = 0.00003f; /* req2uired to compensate for limited float precision */
__constant float PI = 3.14159265359f;
__constant int SAMPLES = 4;

typedef struct Ray{
	float3 origin;
	float3 dir;
} Ray;

typedef struct Sphere{
	float radius;
	float3 pos;
	float3 color;
	float3 emission;
} Sphere;

typedef struct Camera{
	float3 position;		
	float3 view;			
	float3 up;			
	float2 resolution;	
	float2 fov;		
	float apertureRadius;
	float focalDistance;
} Camera;


//static float get_random(unsigned int *seed0, unsigned int *seed1) {
//
//	/* hash the seeds using bitwise AND operations and bitshifts */
//	*seed0 = 36969 * ((*seed0) & 65535) + ((*seed0) >> 16);
//	*seed1 = 18000 * ((*seed1) & 65535) + ((*seed1) >> 16);
//
//	unsigned int ires = ((*seed0) << 16) + (*seed1);
//
//	/* use union struct to convert int to float */
//	union {
//		float f;
//		unsigned int ui;
//	} res;
//
//	res.ui = (ires & 0x007fffff) | 0x40000000;  /* bitwise AND, bitwise OR */
//	return (res.f - 2.0f) / 2.0f;
//}

//Ray createCamRay(const int x_coord, const int y_coord, const int width, const int height, __constant Camera* cam) {
Ray createCamRay(const int x_coord, const int y_coord, const int width, const int height, __global Camera* cam) {

	/* create a local coordinate frame for the camera */
	float3 rendercamview = cam->view; rendercamview = normalize(rendercamview);
	float3 rendercamup = cam->up; rendercamup = normalize(rendercamup);
	float3 horizontalAxis = cross(rendercamview, rendercamup); horizontalAxis = normalize(horizontalAxis);
	float3 verticalAxis = cross(horizontalAxis, rendercamview); verticalAxis = normalize(verticalAxis);

	float3 middle = cam->position + rendercamview;
	float3 horizontal = horizontalAxis * tan(cam->fov.x * 0.5f * (PI / 180)); 
	float3 vertical   =  verticalAxis * tan(cam->fov.y * -0.5f * (PI / 180)); 

	unsigned int x = x_coord;
	unsigned int y = y_coord;

	int pixelx = x_coord; 
	int pixely = height - y_coord - 1;

	float sx = (float)pixelx / (width - 1.0f);
	float sy = (float)pixely / (height - 1.0f);
	
	float3 pointOnPlaneOneUnitAwayFromEye = middle + (horizontal * ((2 * sx) - 1)) + (vertical * ((2 * sy) - 1));
	float3 pointOnImagePlane = cam->position + ((pointOnPlaneOneUnitAwayFromEye - cam->position) * cam->focalDistance); /* cam->focalDistance */

	float3 aperturePoint;
//
//	/* if aperture is non-zero (aperture is zero for pinhole camera), pick a random point on the aperture/lens */
//	if (cam->apertureRadius > 0.00001f) { 
//
//		float random1 = get_random(seed0, seed1);
//		float random2 = get_random(seed1, seed0);
//
//		float angle = 2 * PI * random1;
//		float distance = cam->apertureRadius * sqrt(random2);
//		float apertureX = cos(angle) * distance;
//		float apertureY = sin(angle) * distance;
//
//		aperturePoint = cam->position + (horizontalAxis * apertureX) + (verticalAxis * apertureY);
//	}
//	else aperturePoint = cam->position;

  aperturePoint = cam->position;
	float3 apertureToImagePlane = pointOnImagePlane - aperturePoint; apertureToImagePlane = normalize(apertureToImagePlane); 

	/* create camera ray*/
	Ray ray;
	ray.origin = aperturePoint;
	ray.dir = apertureToImagePlane; 

	return ray;
}

/* (__global Sphere* sphere, const Ray* ray) */
//float intersect_sphere(const Sphere* sphere, const Ray* ray) /* version using local copy of sphere */
//{
//	float3 rayToCenter = sphere->pos - ray->origin;
//	float b = dot(rayToCenter, ray->dir);
//	float c = dot(rayToCenter, rayToCenter) - sphere->radius*sphere->radius;
//	float disc = b * b - c;
//
//	if (disc < 0.0f) return 0.0f;
//	else disc = sqrt(disc);
//
//	if ((b - disc) > EPSILON) return b - disc;
//	if ((b + disc) > EPSILON) return b + disc;
//
//	return 0.0f;
//}

//bool intersect_scene(__constant Sphere* spheres, const Ray* ray, float* t, int* sphere_id, const int sphere_count)
//{
//	/* initialise t to a very large number,
//	so t will be guaranteed to be smaller
//	when a hit with the scene occurs */
//
//	float inf = 1e20f;
//	*t = inf;
//
//	/* check if the ray intersects each sphere in the scene */
//	for (int i = 0; i < sphere_count; i++)  {
//
//		Sphere sphere = spheres[i]; /* create local copy of sphere */
//
//		/* float hitdistance = intersect_sphere(&spheres[i], ray); */
//		float hitdistance = intersect_sphere(&sphere, ray);
//		/* keep track of the closest intersection and hitobject found so far */
//		if (hitdistance != 0.0f && hitdistance < *t) {
//			*t = hitdistance;
//			*sphere_id = i;
//		}
//	}
//	return *t < inf; /* true when ray interesects the scene */
//}


/* the path tracing function */
/* computes a path (starting from the camera) with a defined number of bounces, accumulates light/color at each bounce */
/* each ray hitting a surface will be reflected in a random direction (by randomly sampling the hemisphere above the hitpoint) */
/* small optimisation: diffuse ray directions are calculated using cosine weighted importance sampling */

//float3 trace(__constant Sphere* spheres, const Ray* camray, const int sphere_count, const int* seed0, const int* seed1){
//
//	Ray ray = *camray;
//
//	float3 accum_color = (float3)(0.0f, 0.0f, 0.0f);
//	float3 mask = (float3)(1.0f, 1.0f, 1.0f);
//	int randSeed0 = seed0;
//	int randSeed1 = seed1;
//
//	for (int bounces = 0; bounces < 8; bounces++){
//
//		float t;   /* distance to intersection */
//		int hitsphere_id = 0; /* index of intersected sphere */
//
//		/* update random number seeds for each bounce */ 
//		/*randSeed0 += 1; */ 
//		/*randSeed1 += 345;*/ 
//		
//
//		/* if ray misses scene, return background colour */
//		if (!intersect_scene(spheres, &ray, &t, &hitsphere_id, sphere_count))
//			return accum_color += mask * (float3)(0.15f, 0.15f, 0.25f);
//
//		/* else, we've got a hit! Fetch the closest hit sphere */
//		Sphere hitsphere = spheres[hitsphere_id]; /* version with local copy of sphere */
//
//		/* compute the hitpoint using the ray equation */
//		float3 hitpoint = ray.origin + ray.dir * t;
//
//		/* compute the surface normal and flip it if necessary to face the incoming ray */
//		float3 normal = normalize(hitpoint - hitsphere.pos);
//		float3 normal_facing = dot(normal, ray.dir) < 0.0f ? normal : normal * (-1.0f);
//
//		/* compute two random numbers to pick a random point on the hemisphere above the hitpoint*/
//		float rand1 = get_random(randSeed0, randSeed1) * 2.0f * PI;
//		float rand2 = get_random(randSeed1, randSeed0);
//		float rand2s = sqrt(rand2);
//
//		/* create a local orthogonal coordinate frame centered at the hitpoint */
//		float3 w = normal_facing;
//		float3 axis = fabs(w.x) > 0.1f ? (float3)(0.0f, 1.0f, 0.0f) : (float3)(1.0f, 0.0f, 0.0f);
//		float3 u = normalize(cross(axis, w));
//		float3 v = cross(w, u);
//
//		/* use the coordinte frame and random numbers to compute the next ray direction */
//		float3 newdir = normalize(u * cos(rand1)*rand2s + v*sin(rand1)*rand2s + w*sqrt(1.0f - rand2));
//
//		/* add a very small offset to the hitpoint to prevent self intersection */
//		ray.origin = hitpoint + normal_facing * EPSILON;
//		ray.dir = newdir;
//
//		/* add the colour and light contributions to the accumulated colour */
//		accum_color += mask * hitsphere.emission;
//
//		/* the mask colour picks up surface colours at each bounce */
//		mask *= hitsphere.color;
//
//		/* perform cosine-weighted importance sampling for diffuse surfaces*/
//		mask *= dot(newdir, normal_facing);
//	}
//
//	return accum_color;
//}

//union Colour{ float c; uchar4 components; };

///**
// * Map cube coordinate to the index of the density value array.
// * @param global_x The count of total poinst in x direction.
// * @param global_y The count of total poinst in y direction.
// * @param global_z The count of total poinst in z direction.
// * @param x_offset The offset for density array for y coordinates.
// * @param y_offset The offset for density array for z coordinates.
// * @param return The index of density value array.
// */
//int globalIndex(int global_x, int global_y, int global_z, int x_offset, int y_offset) 
//{
//  return global_x + x_offset * global_y + x_offset * y_offset * global_z;
//}

float maasto(float3 f_pos)
{
  return -1.0f;
 	//float value =  GetWhiteNoise3(3,f_pos.x*0.001, f_pos.y*0.001, f_pos.z*0.001); 
  //return f_pos.y + value*0.1;
}

float anti_maasto(float3 f_pos)
{
  return -1.0f;
  //float m = maasto(f_pos);
  //float value;
  //modf(m, &value);
  //return value;
}

float sceneSDF(float3 cam_ray, float isovalue) {
//  int x = (int)cam_ray.x; 
//  int y = (int)cam_ray.y; 
//  int z = (int)cam_ray.z; 

//  size_t x_dim = get_global_size(0);
//  size_t y_dim = get_global_size(1);
//  size_t z_dim = get_global_size(2);

   return anti_maasto(cam_ray) - isovalue;
}

// TODO: check mc-normal indexes!!! Possible BUG.
float3 calculate_normal(float3 pos)
{
  float3 grad;
  float right = maasto(pos); //   antimaasto((float3){pos.x+1.0f, pos.y,pos.z});
  float left =    maasto((float3){pos.x-1.0f, pos.y,pos.z});
  float up =      maasto((float3){pos.x, pos.y+1.0f,pos.z});
  float down =    maasto((float3){pos.x, pos.y-1.0f,pos.z});
  float z =       maasto((float3){pos.x, pos.y,pos.z-1.0f});
  float z_minus = maasto((float3){pos.x, pos.y,pos.z+1.0f});

  //return (float3){0.0f,0.0f,0.0f};
  grad.x = right - left;
  grad.y = up - down;
  grad.z = z - z_minus;
  return normalize(grad); // Normalize the result. 
}

//__kernel void render_kernel(const float isovalue, const int width, const int height, __global float3* output, __constant const Camera* cam)
__kernel void render_kernel(float isovalue, int width, int height, __global float8* output, __global Camera* cam)
{
	unsigned int work_item_id = get_global_id(0);	/* the unique global id of the work item for the current pixel */
	unsigned int x_coord = work_item_id % width;			/* x-coordinate of the pixel */
	unsigned int y_coord = work_item_id / width;			/* y-coordinate of the pixel */

	Ray camray = createCamRay(x_coord, y_coord, width, height, cam);
  float depth = 0.0f;
  float3 normal = (float3){0.0f,0.0f,0.0f};
  //for (int i=0 ; i<MAX_MARCHING_STEPS; i++) {
  //  depth += 1.0f;
  //  float3 ray_point = camray.origin + (-1.0f) * depth * camray.dir;
  //  if (sceneSDF(ray_point, isovalue) < 0.0f) { normal = calculate_normal(ray_point); break; } 
  //}

	output[work_item_id] = float8(x_coord, y_coord, 0.5f, 0.3f, 0.2f, normal.x,normal.y,normal.z); //; // (float3){x_coord, y_coord, 1.0f};
   
//	/* add the light contribution of each sample and average over all samples*/
//	float3 finalcolor = (float3)(0.0f, 0.0f, 0.0f);
//	float invSamples = 1.0f / SAMPLES;
//
//	for (unsigned int i = 0; i < SAMPLES; i++){
//		finalcolor += trace(spheres, &camray, sphere_count, &seed0, &seed1) * invSamples;
//	}
//
//	/* add pixel colour to accumulation buffer (accumulates all samples) */
//	accumbuffer[work_item_id] += finalcolor;
//	/* averaged colour: divide colour by the number of calculated frames so far */
//	float3 tempcolor = accumbuffer[work_item_id] / framenumber;
//
//	tempcolor = (float3)(
//		clamp(tempcolor.x, 0.0f, 1.0f),
//		clamp(tempcolor.y, 0.0f, 1.0f), 
//		clamp(tempcolor.z, 0.0f, 1.0f));
//
//	union Colour fcolor;
//	fcolor.components = (uchar4)(
//		(unsigned char)(tempcolor.x * 255),
//		(unsigned char)(tempcolor.y * 255),
//		(unsigned char)(tempcolor.z * 255),
//		1);
//
//	/* store the pixelcolour in the output buffer */
//	output[work_item_id] = (float3)(x_coord, y_coord, fcolor.c);
}
