# Image_Compressor
C program to compress a ppm image

To implement this project, two critical data structures were devised to store information. The first structure, a color structure, is used to retain RGB colors extracted from a PPM file. The second structure, a quadtree, is employed to store nodes in the tree. These nodes can represent either leaves containing colors or non-leaf nodes, and they also store their dimensions.

To fulfill the initial requirements, three essential functions were conceived:

A function for node creation.
A function for leaf creation.
A function to transform a matrix of colors into a binary tree.
For node creation, the algorithm initializes a node and establishes links to its four children, named conveniently after the four cardinal directions: north-west, north-east, south-east, and south-west.

Leaf creation initializes a node and adds the colors it contains, along with their coverage area in the matrix.

To create a binary tree from a matrix of RGB colors, an algorithm is employed. It begins with the entire matrix, checking whether the average color within it is smaller than a predefined color factor. In this case, the entire tree will have a single node, acting as a maximum-size leaf. If this case does not apply, the algorithm proceeds to divide the matrix into regions, starting from a specified point (n, m) and traversing half of the matrix's size. This function is recursive, so if a region cannot be transformed into a single color, the algorithm descends into it and reapplies itself.

Additionally, for the first requirement, three recursive functions were created to calculate the tree's height, the number of leaves, and the maximum leaf size.

To address the second requirement, a function was developed to traverse the tree level by level using a queue and its related functions. This function stores nodes to be traversed in the correct order. If a node in the tree is not a leaf, it is represented as '0' in the binary file, and its children are added to the queue. If it is a leaf, '1' is written, followed by the RGB colors of the leaf.

For the third requirement, the function for node initialization in the tree, described earlier, was employed. Two additional functions were created for binary file decompression and data transformation into a quadtree, along with a function to generate a matrix.

The decompression function uses a previously mentioned queue to add tree nodes. Starting with the root, it reads information from the file. If the information is '0' (indicating a non-leaf node), it initializes the node's children and adds them to the queue. If the information is '1', it signifies a leaf node, and the color information is read and added to the node. Two level variables are used to keep track of nodes on the current level and those on the next level, facilitating the calculation of the maximum leaf size in the image.

To create an image, the function starts with the constructed quadtree and checks whether the tree has only one color at the root. In this case, the entire image is a single color. Otherwise, it iterates through the tree's four quadrants, writing color data into the matrix when it reaches leaf nodes.

Finally, in response to the fourth requirement, the matrix generated is added to the PPM file.

The tree release function is called at each stage and involves a recursive traversal of the tree, beginning with leaf release and progressing to the next level until the root is reached.

In the main function, command-line arguments are parsed to open the required files and process each task accordingly. For compression-related tasks (c-type requirements), image compression is performed with the associated sub-tasks. For the decompression task (d-type requirement), the binary file is decompressed, and a PPM image is created using the functions previously described.

Before compression
![image](https://github.com/pachirishu/Image_Compressor/assets/54062247/72f8e19c-6219-4b53-9da8-de0b95dcedab)

After compression
![image](https://github.com/pachirishu/Image_Compressor/assets/54062247/50604de0-7515-420f-b534-3c883453cc4c)

