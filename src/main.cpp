#include "Mesh.hpp"
#include <algorithm>
#include <cstdlib>
#include <vector>

int
usage(int argc, char * argv[])
{
  DGP_CONSOLE << "";
  DGP_CONSOLE << "Usage: " << argv[0] << " <mesh> [vol2bbox] [d2 <#points> <#bins>]";
  DGP_CONSOLE << "";

  return -1;
}

/** Compute the volume enclosed by the mesh. An approximation is ok as long as it is not too coarse. */
double
computeVolume(Mesh const & mesh)
{
  // TODO

  return -1;
}

/** Compute the ratio of the volume of the shape to the volume of its bounding box. */
double
computeVolumeToBBoxRatio(Mesh const & mesh)
{
  // TODO: You only need to complete ONE of the compute*() functions for the basic portion of the assignment.

  // (1) Compute the (approximate) volume enclosed by the mesh using computeVolume() (this is the hard bit)
  // (2) Compute the bbox volume of the mesh  (this is the easy bit -- it's a one-liner)
  // (3) Return the ratio

  return -1;
}

/**
 * Select \a num_points points from the surface of a mesh, distributed i.i.d. and uniformly by area. You do NOT need to
 * implement something fancy like furthest point sampling, just repeat the basic step of:
 *   - pick a face with probability proportional to its area
 *   - pick a random point uniformly distributed across the face
 */
void
samplePoints(Mesh const & mesh, size_t num_points, std::vector<Vector3> & points)
{
  // TODO

  points.clear();
}

/**
 * Compute the Euclidean D2 descriptor: a histogram of all pairwise Euclidean distances between num_points points on the mesh,
 * distributed uniformly and identically by area. The histogram should divide the range of distances between 0 and the bounding
 * box diagonal of the mesh into num_bins uniformly spaced bins. Normalize the histogram by dividing by the total number of
 * points.
 */
void
computeD2(Mesh const & mesh, size_t num_points, size_t num_bins, std::vector<double> & histogram)
{
  // TODO: You only need to complete ONE of the compute*() functions for the basic portion of the assignment.

  histogram.resize(num_bins);
  fill(histogram.begin(), histogram.end(), 0.0);

  // (1) Sample num_points points using samplePoints()
  // (2) Compute the bounding box diagonal, which is the maximum value that can be binned in the histogram
  // (3) Compute all pairwise L2 distances between distinct points (i.e. ignore the zero distance between a point and itself)
  //     and bin them in the histogram
}

/**
 * Smooth a mesh
 * Paper source - Fleishman-2003.pdf
 */
Mesh
bilateralSmoothing(Mesh const & mesh, double sigma_c, double sigma_s)
{
  Mesh smoothMesh = mesh;
  smoothMesh.bilateralSmooth(sigma_c, sigma_s);
  return smoothMesh;
}



int
main(int argc, char * argv[])
{
  if (argc < 2)
    return usage(argc, argv);

  std::string in_path = argv[1];

  Mesh mesh;
  if (!mesh.load(in_path))
    return -1;

  DGP_CONSOLE << "Read mesh '" << mesh.getName() << "' with " << mesh.numVertices() << " vertices, " << mesh.numEdges()
              << " edges and " << mesh.numFaces() << " faces from " << in_path;

  std::vector<double> feature_vector;
  for (int i = 2; i < argc; ++i)
  {
    std::string feat_type = argv[i];
    if (feat_type == "vol2bbox")
      feature_vector.push_back(computeVolumeToBBoxRatio(mesh));
    else if (feat_type == "d2")
    {
      if (i > argc - 3)
        return usage(argc, argv);

      int num_points = atoi(argv[++i]);
      int num_bins = atoi(argv[++i]);
      if (num_points < 1 || num_bins < 1)
      {
        std::cerr << "Invalid D2 parameters" << std::endl;
        return -1;
      }

      std::vector<double> d2;
      computeD2(mesh, (size_t)num_points, (size_t)num_bins, d2);
      feature_vector.insert(feature_vector.end(), d2.begin(), d2.end());
    }
  }

  for (size_t i = 0; i < feature_vector.size(); ++i)
  {
    if (i > 0) std::cout << ' ';
    std::cout << feature_vector[i];
  }

  std::cout << std::endl;

  return 0;
}
