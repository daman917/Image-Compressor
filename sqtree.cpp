/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  // Your code here.

  stats s(imIn);
  pair<int, int> origin(0, 0);
  root = buildTree(s, origin, imIn.width(), imIn.height(), tol);

}

/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
  // Your code here.

  Node * res = new Node(s,ul,w,h);

  if(w==1 && h==1){
    return res;
  }

  if (s.getVar(ul,w,h)<=tol) {
    return res;
  }
  
  pair<int, int> split_axis  = split(s,ul,w,h);
  int i=split_axis.first;
  int j=split_axis.second;

  int x = ul.first;
  int y = ul.second;

  pair<int, int> NWQUAD(x, y);
  pair<int, int> SWQUAD(x, y+j);
  pair<int, int> NEQUAD(x+i, y);
  pair<int, int> SEQUAD(x+i, y+j);


  if(i==0){
    res->NW = buildTree(s,NWQUAD, w,j,tol);
    res->SW =  buildTree(s,SWQUAD, w,h-j,tol);
  } 
  
  if(j==0){
    res->NW = buildTree(s,NWQUAD, i,h,tol);
    res->NE =  buildTree(s,NEQUAD, w-i,h,tol);
  }

  if (i!=0 && j!=0) {
    res->NW = buildTree(s,NWQUAD, i,j,tol);
    res->NE =  buildTree(s,NEQUAD, w-i,j,tol);
    res->SW =  buildTree(s,SWQUAD, i,h-j,tol);
    res->SE =  buildTree(s,SEQUAD, w-i,h-j,tol);
  }

  return res;

}


pair<int,int> SQtree::split(stats & s, pair<int,int> & ul,
				 int w, int h) {
           pair<int, int> split;
           pair<int, int> prev_split;

           int x = ul.first;
           int y = ul.second;

           double prev_max = s.getVar(ul,w,h);


           for(int i=0; i<w; i++){
             for(int j=0;j<h; j++){
               if(i==0&&j==0) continue;
               double split_max  =  0;
               split.first=i;
               split.second=j;
               if(s.getVar(ul,i,j)>split_max){
                 split_max =  s.getVar(ul,i,j);
               }
               if(s.getVar(pair<int,int>(x+i,y),w-i,j)>split_max){
                split_max =  s.getVar(pair<int,int>(x+i,y),w-i,j);
               }
               if(s.getVar(pair<int,int>(x,y+j),i,h-j)>split_max){
                split_max =  s.getVar(pair<int,int>(x,y+j),i,h-j);
               }
               if(s.getVar(pair<int,int>(x+i,y+j),w-i,h-j)>split_max){
                split_max =  s.getVar(pair<int,int>(x+i,y+j),w-i,h-j);
               }

               if(split_max<prev_max){
                 prev_max = split_max;
                 prev_split.first=split.first;
                 prev_split.second=split.second;
               }
             }
           }     
           return prev_split;

         }


  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  // Your code here.
  PNG im(root->width, root->height);

  vector<Node*> imNodes = getImNodes(root);

  for (int i = 0; i < imNodes.size(); i++) {
    for (int j = 0; j < imNodes[i]->width; j++) {
      for (int k = 0; k < imNodes[i]->height; k++) {
        *im.getPixel(imNodes[i]->upLeft.first + j, imNodes[i]->upLeft.second + k) = imNodes[i]->avg;
      }
    }
  }

  return im;
}

vector<SQtree::Node*> SQtree::getImNodes(Node* r) {
  vector<Node*> imNodes;

  if (r->NW) {
    vector<Node*> nwNodes = getImNodes(r->NW);
    imNodes.insert(imNodes.end(), nwNodes.begin(), nwNodes.end());
  }

  if (r->NE) {
    vector<Node*> neNodes = getImNodes(r->NE);
    imNodes.insert(imNodes.end(), neNodes.begin(), neNodes.end());
  }

  if (r->SE) {
    vector<Node*> seNodes = getImNodes(r->SE);
    imNodes.insert(imNodes.end(), seNodes.begin(), seNodes.end());
  }

  if (r->SW) {
    vector<Node*> swNodes = getImNodes(r->SW);
    imNodes.insert(imNodes.end(), swNodes.begin(), swNodes.end());
  }

  if (!r->NW && !r->NE && !r->SE && !r->SW) {
    imNodes.push_back(r);
  }

  return imNodes;
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  // Your code here.
  // SQtree copy constructor, given.
  clearer(root);
}

/**
 * Delete allocated memory.
 */
void SQtree::clearer(Node *root) {
  // Your code here.
  // SQtree copy constructor, given.
  if(root==NULL){
    return;
  }
  clearer(root->NW);
  clearer(root->NE);
  clearer(root->SW);
  clearer(root->SE);

  delete root;
  


}

void SQtree::copy(const SQtree & other) {
  // Your code here.
  root = copyNode(other.root);
}


SQtree::Node* SQtree::copyNode(Node* r) {
  Node * newNode = new Node(r->upLeft, r->width, r->height, r->avg, r->var);

  if (r->NW) {
    newNode->NW = copyNode(r->NW);
  }

  if (r->NE) {
    newNode->NE = copyNode(r->NE);
  }
  
  if (r->SE) {
    newNode->SE = copyNode(r->SE);
  }

  if (r->SW) {
    newNode->SW = copyNode(r->SW);
  }

  return newNode;
}
int SQtree::sizeHelper(Node *subroot) {
  // Your code here.
  if(subroot==NULL){
    return 0;
  }

  return sizeHelper(subroot->NW)+sizeHelper(subroot->NE)+sizeHelper(subroot->SE)+sizeHelper(subroot->SW) + 1;

}
int SQtree::size() {
  // Your code here.
  return  sizeHelper(root);
}