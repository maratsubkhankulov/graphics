CAV coursework 1
Marat Subkhankulov s0806287

I've implemented matrix classes to support matrix multiplication and matrix-vector multiplication to skin the model.

Controls:

To choose joint:
O, P - cycles through the joints
To rotate chosen joint:
Q,E - rotate in x axis
A,D - rotate in y axis
S,W - rotate in z axis



#compiling the demo program:

g++ -o view view.cc -lglut -lGLU -lGL

# running the program

./view arma2.obj   


# about  skeleton.out  and attachment.out 

Each line in skeleton.out corresponds to a joint and is of the form:
idx x y z prev where prev is the index of the previous joint.

The skeletons are composed of 21 bones (22 joints).
The ID of the bone is same as the ID of the joint in skeleton2.out. 
In attachment2.out, the weights for each bone is listed.    

Each line in attachment.out corresponds to a mesh vertex and
consists of bone weights for each bone in the order of bone IDs.
