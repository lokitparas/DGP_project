#include "MeshVertex.hpp"
#include "MeshEdge.hpp"
#include "MeshFace.hpp"

MeshEdge *
MeshVertex::getEdgeTo(MeshVertex const * v)
{
  if (v == this) return NULL;

  for (EdgeConstIterator ei = edgesBegin(); ei != edgesEnd(); ++ei)
  {
    Edge * e = *ei;
    if (e->hasEndpoint(v)) return e;
  }

  return NULL;
}

bool
MeshVertex::isBoundary() const
{
  if (edges.empty()) return true;

  for (EdgeConstIterator ei = edgesBegin(); ei != edgesEnd(); ++ei)
    if ((*ei)->isBoundary()) return true;

  return false;
}

void
MeshVertex::addFace(Face * face, bool update_normal)
{
  faces.push_back(face);
  if (update_normal && !has_precomputed_normal)
    addFaceNormal(face->getNormal());
}

void
MeshVertex::removeFace(Face * face)
{
  for (FaceIterator fi = facesBegin(); fi != facesEnd(); )
  {
    if (*fi == face)
    {
      fi = faces.erase(fi);
      if (!has_precomputed_normal) removeFaceNormal(face->getNormal());

      // Keep going, just in case the face somehow got added twice
    }
    else
      ++fi;
  }
}

void
MeshVertex::updateNormal()
{
  if (!faces.empty())
  {
    Vector3 sum_normals = Vector3::zero();
    for (FaceConstIterator fi = faces.begin(); fi != faces.end(); ++fi)
      sum_normals += (*fi)->getNormal();  // weight by face area?

    normal_normalization_factor = sum_normals.length();
    setNormal(normal_normalization_factor < 1e-20f ? Vector3::zero() : sum_normals / normal_normalization_factor);
  }
  else
  {
    setNormal(Vector3::zero());
    normal_normalization_factor = 0;
  }

  has_precomputed_normal = false;
}

std::list<MeshFace *> 
MeshVertex::findNeighbourPlanes(double sigma_c)
{
  std::list<MeshFace * > neighbours;
  std::queue<MeshFace *> q;

  // this->isCovered = true;
  // q.push(this);

  for (FaceIterator it = this->facesBegin(); it != this->facesEnd(); ++it)
  {
    q.push(*it);
    neighbours.push_back(*it);
    (*it)->isCovered = true;
  }

  while(!q.empty()){
    MeshFace * curr = q.front();
    q.pop();

    for( MeshFace::VertexIterator it = curr->verticesBegin(); it !=  curr->verticesEnd(); ++it){
        
        for (FaceIterator it2 = (*it)->facesBegin(); it2 != (*it)->facesEnd(); ++it2)
        {
          Vector3 centroid = (*it2)->getCentroid();
          double dist = (this->getPosition() - centroid).length();
          if(!(*it2)->isCovered && dist < 2*sigma_c){
            q.push(*it2);
            neighbours.push_back(*it2);
            (*it2)->isCovered = true;  
          }
        }
        // MeshVertex* v1 = (*it)->getOtherEndpoint(curr);
        // double distance = (v1->getPosition()-this->getPosition()).length();
        // if (!v1->isCovered){
        //   v1->isCovered = true;
        //   if(distance < 2*sigma_c){
        //     q.push(v1);
        //   }
        //   neighbours.push_back(v1);
        // }
    }
  }

  return neighbours;
}


std::list<MeshVertex *> 
MeshVertex::findNeighbourVertices(double sigma_c)
{
  std::list<MeshVertex * > neighbours;
  std::queue<MeshVertex *> q;

  this->isCovered = true;
  q.push(this);


  while(!q.empty()){
    MeshVertex * curr = q.front();
    q.pop();

    for( EdgeIterator it = curr->edgesBegin(); it !=  curr->edgesEnd(); ++it){
        MeshVertex* v1 = (*it)->getOtherEndpoint(curr);
        double distance = (v1->getPosition()-this->getPosition()).length();
        if (!v1->isCovered){
          v1->isCovered = true;
          if(distance < 2*sigma_c){
            q.push(v1);
          }
          neighbours.push_back(v1);
        }
    }
  }

  return neighbours;
}
