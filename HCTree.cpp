#include "HCTree.hpp"
#include <stack>
#include <iostream>
using namespace std;
/** Cautions/Bugs to be wary about: 
  * Seg Fault for Destructor Method
  * 
  *
  *
 */

void outputSerialize(HCNode* node, FancyOutputStream &out){
    if(node != nullptr)
    {
        outputSerialize(node->c0,out);
        // out.write_int(node->count);
        outputSerialize(node->c1,out);
    }
    return;
}   

void inputDeserialize(HCNode* node, FancyInputStream &in){
    if(node != nullptr){
        inputDeserialize(node->c0,in);
        // in.read_int(node->count);
        inputDeserialize(node->c1,in);
    }
}
void HCTree::serializingTree(FancyOutputStream &out){
    outputSerialize(this->root,out);
}

void HCTree::deserializingTree(FancyInputStream &in){
    inputDeserialize(this->root,in);
}

void treeTraversing(HCNode* node){
    if(node != nullptr){
        treeTraversing(node->c0);
        treeTraversing(node->c1);
        cout << "Node Symbol: " << +node->symbol << " Node Count: " << node->count << endl;
    }
    return;
}
void HCTree::treeTraversal(){
    treeTraversing(this->root);
}
//Helper Method to traverse Huffman Coding Tree
//Post-Order traversal
void traversal(HCNode* node)
{
    if(node != nullptr){
        traversal(node->c0);
        traversal(node->c1);
        delete(node);
        node = nullptr;
    }
    return;
}

//Destructor
HCTree::~HCTree(){
    traversal(root);
    this->leaves.clear();
    return;
}

void HCTree::build(const vector<int>& freqs){
    //Root has smallest element, and then going down has higher frequencies (min-heap) of a pq 
    
    //Use a PQ to order the elements from smallest to greatest 
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    
    //Need to create HCNode's for each symbol in frequency vector 
    //HCNode must have an int for the count but also an unsigned char to represent the symbol of it. 

    //Creating nodes for each symbol+count and sorting them with a pq
    for(unsigned int i = 0; i < freqs.size(); i++){
        if(freqs[i] > 0){
            HCNode* node = new HCNode(freqs[i], i);
            leaves[i] = node;
            pq.push(node);
        }
    }

    //Now we want to construct the actual Tree
    //We select two lowest counts, (should be first and second item of PQ), and create a new parent 
    //node for those two nodes

    while(pq.size() > 1 )
    {
        HCNode *smallest = pq.top();
        pq.pop();
        HCNode *secondSmallest = pq.top();
        pq.pop();
        HCNode *parentNode = new HCNode(smallest->count + secondSmallest->count , smallest->symbol);
        smallest->p = parentNode;
        secondSmallest->p = parentNode;
        parentNode->c1 = secondSmallest; 
        parentNode->c0 = smallest;
        pq.push(parentNode);
    }

    if(!pq.empty())
    {
        this->root = pq.top();
        pq.pop();
    }
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{

    HCNode* curr = leaves[symbol]; 
    stack<int> bitHolder;

    while(curr != root)
    {
        HCNode* parentNode = curr->p;

        if(parentNode->c0 == curr)
        {
            bitHolder.push(0);
        }
        else if(parentNode->c1 == curr){
            bitHolder.push(1);
        }
        curr = parentNode;
    }
    while(!bitHolder.empty())
    {   
        // int size = bitHolder.size();
        int bit = bitHolder.top();
        bitHolder.pop();
        out.write_bit(bit); 
    }
}   

/* Error: in.readbit not correctly reading in bits?
 * When encoding symbol '2' we write out 111 as our encoded bit sequence
 * When decoding, however we only read in 10, which gets a wrong symbol.
*/
int HCTree::decode(FancyInputStream & in) const { //unsigned char
    HCNode* curr = root;
    if(curr == nullptr)
    {
        return 0;
    }
    while(curr->c0 != nullptr || curr->c1 != nullptr){
        int bit = in.read_bit();
        if(bit == -1){
            //If no more bits to read, reached EOF, 
            // break;
            // return 255;
            return -1;
            /* */
            //If no more bits read, but bit buffer isn't at 0,
            // we just want to append 0's?
        }

        else if(bit == 1)
        {
            curr = curr->c1;
        }
        else if(bit == 0){
            curr = curr->c0;
        }
    }
    return curr->symbol;

}
