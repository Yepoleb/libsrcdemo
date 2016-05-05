/*
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 2 of the License, or (at
*  your option) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software Foundation,
*  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

/**
 *  @author     commonbullet
 *  @version    1.05a
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "vdfparser.hpp"


/*
 *  Macro for vdf indentation
 */
#define TABFILL(tabStr, tabNum)\
    memset(tabStr, '\t', tabNum);\
    tabStr[tabNum] = '\0';\


template <typename T>
void Finalize(T &pTarget)
{
    if(pTarget == NULL)
        return;

    delete (T*) pTarget;
    pTarget = NULL;
}

template <typename T>
void FinalizeArray(T &pTarget)
{
    if(pTarget == NULL)
        return;

    delete [] pTarget;
    pTarget = NULL;
}

/**
 *  Adds a slot to a pointer array
 *  @param  target      target pointer Array
 *  @param  curLength   current size of pointer Array
 */
template <typename T>
void GrowPArray(T ***target, size_t &curLength)
{
    T **cache;

    cache = *target;

    *target = new T*[sizeof(T*) * curLength + 1];

    if(curLength)
    {
        memcpy(*target, cache, curLength * sizeof(T*));
        FinalizeArray(cache);
    }

    (*target)[curLength] = NULL;
}


/**
 *  Convers a c string to lower case.
 *  @param  src     Source string.
 *  @param  dest    Target string.
 */
void ToLowerCase(char *src, char *dest)
{
    while(*src)
        *dest++ = tolower(*(src++));
    (*dest) = '\0';
}


// --- VDFTree class implementation ---

VDFTree::VDFTree()
{
    nodeCount    =  0;
    rootNode     =  NULL;
    nodeIndex    =  NULL;
    treeId       =  0;
}

VDFTree::~VDFTree()
{
    DestroyTree();
}


/**
 *  Initializes tree root node
 */
void VDFTree::CreateTree()
{
    if(rootNode != NULL)
        DestroyTree();
    rootNode = CreateNode();
}

/**
 *  Creates an empty node and adds it to node index
 *
 *  @param  parentNode  node's parent, not required (default = NULL)
 *  @return             The created node or NULL if it fails.
 */
VDFNode *VDFTree::CreateNode(VDFNode *parentNode)
{
    VDFNode  *node;

    GrowPArray(&nodeIndex, nodeCount);

    node = new VDFNode;

    node->childNode     = NULL;
    node->nextNode      = NULL;
    node->parentNode    = NULL;
    node->index         = (int)nodeCount;
    node->key           = NULL;
    node->value         = NULL;
    node->parentNode    = parentNode;
    node->previousNode  = NULL;

    nodeIndex[nodeCount] = node;

    if(node)
        nodeCount++;
    return node;
}

/**
 *  Appends a new node into a given node level
 *
 *  @param  Node        pointer to node to get the level
 *  @param  newNode     pointer to node to be added
 */
void VDFTree::AppendNode(VDFNode *Node, VDFNode *newNode)
{
    VDFNode *lastNode;

    if(Node == NULL)
        return;

    lastNode = GetLastNode(Node);
    lastNode->nextNode = newNode;
    newNode->parentNode = lastNode->parentNode;
    newNode->previousNode = lastNode;
}

/**
 *  Adds a child node into a given node.
 *
 *  @param  Node        node pointer which the child will be added,
 *                      if the node already has a child,<br>
 *                      it's appended to node's child level.
 *  @param  childNode   pointer to the noded to be appended as a child
 */
void VDFTree::AppendChild(VDFNode *Node, VDFNode *childNode)
{
    if(Node == NULL)
        return;

    childNode->parentNode = Node;

    if(Node->childNode)
        AppendNode(Node->childNode, childNode);
    else
        Node->childNode = childNode;
}

/**
 *  Tree memory freeing.
 */
void VDFTree::DestroyTree()
{
    UINT ind;
    VDFNode *node;

    for(ind = 0; ind < nodeCount; ind++) {
        node = *(nodeIndex + ind);
        if(!node)
            continue;
        FinalizeArray(node->key);
        FinalizeArray(node->value);
        delete(node);
    }

    delete [] (nodeIndex);
    nodeCount = 0;
}

/**
 *  Get the node in tree index
 *
 *  @param  id      index of the node.
 *  @return         The target node or NULL if not valid or not found.
 */
VDFNode *VDFTree::GetNodeById(UINT id)
{
    if(id < nodeCount)
        return nodeIndex[id];
    return NULL;
}

/**
 *  Gets the node next to a given one.
 *
 *  @param  Node        node to get the next one.
 *  @return             Returns the next node or NULL if it doesn't
 *                      have a next node.
 */
VDFNode *VDFTree::GetNextNode(VDFNode *Node)
{
    return Node->nextNode;
}

/**
 *  Gets the first node from a given node level.
 *
 *  @param  Node        node to get level in tree
 *  @return             The first node in that level.
 */
VDFNode *VDFTree::GetFirstNode(VDFNode *Node)
{
    VDFNode *firstNode;
    firstNode = (Node->parentNode) ? Node->parentNode->childNode : VDFTree::GetRootNode(Node);
    return firstNode;
}

/**
 *  Gets the last node from a given node level
 *
 *  @param  Node        node to get level in tree
 *  @return             The last node in that level.
 */
VDFNode *VDFTree::GetLastNode(VDFNode *Node)
{
    VDFNode *lastNode;

    lastNode = Node;

    while(true) {
        if(!lastNode->nextNode)
            break;
        lastNode = lastNode->nextNode;
    }
    return lastNode;
}

/**
 *  Although the tree object has a 'rootNode' pointer, this static
 *  function is designed to<br>find the rootNode without the
 *  vdf tree information.
 *
 *  @param  Node        Node to get the previous one.
 *  @return             The previous node or NULL if there's no previous.
 */
VDFNode *VDFTree::GetRootNode(VDFNode *Node)
{
    VDFNode *rootNode;
    rootNode = Node;
    while(rootNode->parentNode)
        rootNode = rootNode->parentNode;
    while(rootNode->previousNode)
        rootNode = rootNode->previousNode;
    return rootNode;
}

/**
 *  Gets the previous node from a given node.
 *
 *  @param  Node        Node to get the previous one.
 *  @return             The previous node or NULL if there's no previous.
 */
VDFNode *VDFTree::GetPreviousNode(VDFNode *Node)
{
    return Node->previousNode;
}

/**
 *  Gets the number of a nodes on a given node.
 *
 *  @param  refNode     Node to check level.
 *  @return             The previous node or NULL if there's no previous.
 */
size_t VDFTree::CountBranchNodes(VDFNode *refNode)
{
    VDFNode *firstNode;
    size_t counter;

    firstNode = GetFirstNode(refNode);
    counter = 0;

    while(firstNode) {
        counter++;
        firstNode = firstNode->nextNode;
    }

    return counter;
}

/**
 *  Sorts nodes in a branch.
 *  @param  refNode     Pick up the branch of this node
 *  @param  byKey       If true sort nodes by key, otherwise use values.
 *  @param  byNumber    If true keys/values are considered as numbers (integers) for sorting.
*/
void VDFTree::SortBranchNodes(VDFNode *refNode, bool byKey, bool byNumber)
{
    VDFNode *firstNode;
    VDFNode *parentNode;
    VDFNode *Node;
    VDFNode **sort;
    size_t length;
    int  ind;
    char *comp1;
    char *comp2;
    int  guide;
    int  p,q;
    int p1;
    bool comp;



    firstNode = VDFTree::GetFirstNode(refNode);

    if(firstNode == NULL)
        return;

    parentNode = firstNode->parentNode;
    length = VDFTree::CountBranchNodes(firstNode);

    if(!length)
        return;

    sort = new VDFNode*[length];
    memset(sort, 0, length * sizeof(VDFNode*));

    Node = firstNode;
    ind = 0;

    while(Node) {
        sort[ind++] = Node;
        Node = Node->nextNode;
    }


    // shell sort
    guide = 1;

    while(guide < (int)length)
        guide *= 2;

    while(guide /= 2) {

        for(p = 0; p < (int)length - guide; p++) {

            p1 = p;

            while(p1 > -1) {

                q = p1 + guide;

                if(q >= (int)length)
                    break;


                comp2 = (byKey) ? sort[p1]->key : sort[p1]->value;
                comp1 = (byKey) ? sort[q]->key : sort[q]->value;

                if(comp1 == NULL)
                    continue;

                comp = (comp2 == NULL) ? false : (!byNumber) ? (strcmp(comp1, comp2) < 0)
                        : atoi(comp1) < atoi(comp2);

                if(comp) {
                    Node = sort[q];
                    sort[q] = sort[p1];
                    sort[p1] = Node;
                }
                p1 -= guide;
            }
        }
    }

    for(ind = 0; ind < (int)length; ind++) {
        sort[ind]->nextNode = (ind + 1 < (int)length) ? sort[ind + 1] : NULL;
        sort[ind]->previousNode = (ind - 1 >= 0) ? sort[ind - 1] : NULL;
    }

    if(parentNode)
        parentNode->childNode = sort[0];
    else if(firstNode == rootNode)
        this->rootNode = sort[0];

    delete(sort);

}

bool VDFTree::IsTreeNode(VDFNode *node) {

    size_t count;

    for(count = 0; count < this->nodeCount; count++) {
        if(this->nodeIndex[count] == node)
            return true;
    }

    return false;

}

void VDFTree::MoveAsChild(VDFNode *parentNode, VDFNode *moveNode)
{
    VDFNode *childNode;
    VDFNode *moveNext;
    VDFNode *moveParent;

    if( parentNode == NULL ||
        moveNode == NULL ||
        moveNode->parentNode == parentNode)
        return;

    if(!IsTreeNode(moveNode) || !IsTreeNode(parentNode))
        return;

    moveNext = moveNode->nextNode;
    moveParent = moveNode->parentNode;
    childNode = parentNode->childNode;

    if(childNode == NULL) {
        parentNode->childNode = moveNode;
        moveNode->nextNode = NULL;
        moveNode->previousNode = NULL;
    }

    else {
        childNode = VDFTree::GetLastNode(childNode);
        childNode->nextNode = moveNode;
        moveNode->nextNode = NULL;
        moveNode->previousNode = childNode;
    }

    moveNode->parentNode = parentNode;

    if(moveParent) {
        if(moveNext) {
            moveParent->childNode = moveNext;
            moveNext->previousNode = NULL;
        }
        else
            moveParent->childNode = NULL;
    }
}

/**
 *  Moves a node to other branch.
 *  @param  refNode     Reference node.
 *  @param  moveNode    Node to be moved.
 *  @param  byNumber    If true keys/values are considered as numbers (integers) for sorting.
*/
void VDFTree::MoveToBranch(VDFNode *refNode, VDFNode *moveNode, UINT position)
{
    VDFNode *targetPrevious;
    VDFNode *targetNext;
    VDFNode *srcPrevious;
    VDFNode *srcNext;

    if(refNode == NULL || moveNode == NULL || refNode == moveNode)
        return;

    if(!IsTreeNode(refNode) || !IsTreeNode(moveNode))
        return;

    targetPrevious = refNode->previousNode;
    targetNext = refNode->nextNode;
    srcPrevious = moveNode->previousNode;
    srcNext = moveNode->nextNode;

    if(position == VDF_MOVEPOS_AFTER) {

        if(srcPrevious == NULL) {
            if(moveNode->parentNode)
                moveNode->parentNode->childNode = srcNext;
        }

        refNode->nextNode = moveNode;
        moveNode->previousNode = refNode;
        moveNode->nextNode = targetNext;
        if(targetNext != NULL)
            targetNext->previousNode = moveNode;

        if(moveNode == rootNode)
            rootNode = GetRootNode(moveNode);

    }
    else {
        if(srcPrevious == NULL) {
            if(moveNode->parentNode)
                moveNode->parentNode->childNode = srcNext;
        }

        refNode->previousNode = moveNode;
        moveNode->nextNode = refNode;
        moveNode->previousNode = targetPrevious;

        if(moveNode == rootNode)
            rootNode = GetRootNode(moveNode);
        else if(refNode == rootNode)
            rootNode = moveNode;

    }

    if(srcNext)
        srcNext->previousNode = srcPrevious;

    if(srcPrevious)
        srcPrevious->nextNode = srcNext;

}

/**
 *  Gets the node depth in a tree
 *  @param  Node    Node to check.
 *  @return         Node depth.
 */
size_t VDFTree::GetNodeLevel(VDFNode *Node)
{
    size_t counter;

    counter = 0;

    while((Node = Node->parentNode))
        counter++;

    return counter;
}

/**
 *  Deletes a node in a given tree's node index.
 *
 *  @param  index   Index of the node to be deleted.
 */
void VDFTree::DeleteNode(UINT index)
{
    if(index < nodeCount) {
        if(nodeIndex[index])
            DeleteNode(nodeIndex[index]);
    }
}

/**
 *  Deletes a node in vdf tree.
 *
 *  @param  Node    Node to be deleted.
 */
void VDFTree::DeleteNode(VDFNode *Node)
{
    VDFNode *temp;
    VDFNode *next;

    temp = Node;

    while(Node != NULL && temp != NULL) {
        if(temp->childNode) {
            temp = temp->childNode;
            continue;
        }

        if(temp == Node)
        {
            if( temp->parentNode ) {
                if(temp->parentNode->childNode == temp) {
                    temp->parentNode->childNode = temp->nextNode;
                }
            }
            if(temp->nextNode) {
                temp->nextNode->previousNode = temp->previousNode;
                //if(Node == rootNode) rootNode = temp->nextNode;
            } //else if(Node == rootNode) rootNode = this->CreateNode();
            if(temp->previousNode) {
                temp->previousNode->nextNode = temp->nextNode;
            }
            next = NULL;
        } else {
            if( temp->parentNode ) {
                temp->parentNode->childNode = temp->nextNode;
            }
            if( temp->nextNode ) {
                temp->nextNode->previousNode = NULL;
                next = temp->nextNode;
            } else {
                next = temp->parentNode;
            }
        }

        nodeIndex[temp->index] = NULL;
        if(temp != rootNode) {
            FinalizeArray(temp->key);
            FinalizeArray(temp->value);
            Finalize(temp);
        }

        temp = next;


    }
}

/**
 *  Gets the actual number of nodes in a tree.
 *
 *  @return     The number of nodes in a tree.
 */
size_t VDFTree::GetLength()
{
    UINT ind;
    size_t length;
    length = 0;
    for(ind = 0; ind < nodeCount; ind++) {
        if(nodeIndex[ind])
            length++;
    }
    return length;
}


/**
 *  Sets key/value in a tree. Use NULL if you need to set
 *  only key or only value.
 *
 *  @param  Node    Target node
 *  @param  key     Node key, if NULL it's ignored.
 *  @param  value   Node value, if NULL it's ignored.
 */
void VDFTree::SetKeyPair(VDFNode *Node, const char *key, const char *value)
{
    if(key) {
        if(Node->key)
            delete(Node->key);
        Node->key = new char[strlen(key) + 1];
        strcpy(Node->key, key);
    }

    if(value) {
        if(Node->value)
            delete(Node->value);
        Node->value = new char[strlen(value) + 1];
        strcpy(Node->value, value);
    }
}


// --- VDFTokenizer class implementation ---

VDFTokenizer::VDFTokenizer()
{
    source = NULL;
    token = NULL;
    pos = 0;
}

VDFTokenizer::~VDFTokenizer()
{
    FinalizeArray(token);
}

/**
 *  Updates token pointer to the next token.
 *
 *  @return Returns true if there are more tokens.
 */
bool VDFTokenizer::HasMoreTokens()
{
    size_t endpos;
    size_t startpos;
    size_t length;
    size_t srclength;

    bool insideQuotes;
    bool hasStarted;


    endpos = pos;
    insideQuotes = false;
    hasStarted = false;
    startpos = 0;
    length = 0;
    srclength = strlen(source);

    if(endpos >= srclength)
        return false;

    FinalizeArray(token);

    while(true) {
        while(endpos < srclength) {
            if(source[endpos] == '\"') {
                if(!insideQuotes && !hasStarted) {
                    insideQuotes = true;
                    hasStarted = true;
                    startpos = endpos + 1;
                }
                else
                    break;
            }

            else if(source[endpos] == ' ' || source[endpos] == '\t') {
                if(!insideQuotes && hasStarted)
                    break;
            }
            else if(isspace(source[endpos])) {
                if(hasStarted)
                    break;
            }
            else {
                if(!hasStarted) {
                    hasStarted = true;
                    startpos = endpos;
                }
            }
            endpos++;
        }
        length = endpos - startpos;
        if(length || endpos >= srclength)
            break;
        else  {
            hasStarted = false;
            endpos++;
        }
    }

    if(length && hasStarted) {
        token = new char[length + 1];
        memcpy(token, (source + startpos), length);
        token[length] = '\0';
    }
    pos = endpos + 1;
    return token != NULL;

}

/**
 *  Sets a new string to be parsed, resets token.
 */
void VDFTokenizer::SetString(char *str)
{
    pos = 0;
    FinalizeArray(token);
    token = NULL;
    source = str;
}


// --- VDFTreeParser class implementation ---

VDFTreeParser::VDFTreeParser()
{
}

VDFTreeParser::~VDFTreeParser()
{
}

/**
 *  Parses a tree using a simple event model - no tree object is created.
 *  @param  filename    Name of the tree to be parsed
 *  @param  pFW         Object containing forwards information.
 */
bool VDFTreeParser::ParseVDF(const char *filename, ParseForward *pFW)
{
    FILE            *pFile;
    char            line[MAX_LINE_SIZE];
    char            key[256];
    char            value[256];
    size_t          argnum;
    VDFTokenizer    tokenizer;
    int             len;
    int             level;
    bool            newChild;
    int             retVal;

    if(pFW == NULL)
        return false;

    if(filename == NULL || pFW->pfnParser == NULL)
        return false;

    pFile = NULL;
    pFile = fopen(filename, "r");
    if(pFile == NULL)
        return false;


    level = 0;
    newChild = false;
    argnum = 0;

    retVal = RETURN_VDFPARSER_CONTINUE;

    if(pFW->pfnStart != NULL)
        (*(pFW->pfnStart))(pFW->fwidStart, pFW->mdFilename);

    while(fgets(line, MAX_LINE_SIZE, pFile)) {

        if(argnum == 1)
            retVal = (*(pFW->pfnParser))(pFW->fwidParser, pFW->mdFilename, key, value, level);

        argnum = 0;
        *key = '\0';
        *value = '\0';

        if(retVal == RETURN_VDFPARSER_STOP)
                break;

        tokenizer.SetString(line);

        while(tokenizer.HasMoreTokens()) {

            if(retVal == RETURN_VDFPARSER_STOP)
                break;

            if((len = strlen(tokenizer.token))) {


                // block comments
                if(tokenizer.token[0] == '/' && tokenizer.token[1] == '/')
                    break;

                // open node
                else if(!strcmp(tokenizer.token, "{")) {

                    //retVal = (*pfnParse)(key, value, level);

                    level++;
                    argnum = 0;
                    newChild = false;
                    *key = '\0';
                    *value = '\0';
                }

                // close node
                else if(!strcmp(tokenizer.token, "}")) {

                    //if(!newChild)
                    //      retVal = (*pfnParse)(key, value, level);

                    if(level) {
                        level--;
                        argnum = 0;
                        newChild = true;
                        *key = '\0';
                        *value = '\0';
                    }
                }

                else {
                    if(!argnum) {
                        if(newChild)
                            newChild = false;
                        strncpy(key, tokenizer.token, 255);
                    }
                    else {
                        newChild = true;
                        strncpy(value, tokenizer.token, 255);
                        retVal = (*(pFW->pfnParser))(pFW->fwidParser, pFW->mdFilename, key, value, level);
                    }
                    argnum++;
                }
            }
        }
    }

    if(argnum == 1 && retVal == RETURN_VDFPARSER_CONTINUE)
        retVal = (*(pFW->pfnParser))(pFW->fwidParser, pFW->mdFilename, key, value, level);

    if(pFW->pfnEnd != NULL)
        (*(pFW->pfnEnd))(pFW->fwidEnd, pFW->mdFilename);

    fclose(pFile);

    return true;

}


/**
 *  Opens an existing vdf file
 *
 *  @param  char*       Name of the vdf file
 *  @param  VDFTree**   The vdf tree will be built into this object.
 *  @return             Returns true if succeeded.
 */
bool VDFTreeParser::OpenVDF(const char *filename, VDFTree **vdfTree, OpenForward *openFW)
{
    FILE     *pFile;
    char     line[MAX_LINE_SIZE];
    int      level;
    int      argnum;
    int      len;
    bool     newChild;
    int      retVal;

    VDFNode  *parent;
    VDFNode  *node;
    VDFNode  *tmpNode;
    VDFTokenizer tokenizer;

    PFN_VDFOPEN pfnParse = NULL;

    if(openFW != NULL)
        pfnParse = openFW->pfnOpen;

    if(filename == NULL) {
        return false;
    }

    pFile = NULL;
    if(!(pFile = fopen(filename, "r")))
        return false;

    if(*vdfTree) {
        delete(*vdfTree);
        *vdfTree = NULL;
    }
    *vdfTree = new VDFTree;

    level  = 0;
    argnum = 0;
    newChild = false;
    (*vdfTree)->CreateTree();
    parent = NULL;
    node   = (*vdfTree)->rootNode;
    retVal = RETURN_TREEPARSER_CONTINUE;

    while(fgets(line, MAX_LINE_SIZE, pFile)) {

        if(retVal == RETURN_TREEPARSER_SILENT)
            pfnParse = NULL;
        else if(retVal == RETURN_TREEPARSER_BREAK)
            break;

        if(argnum == 1 && pfnParse != NULL)
            (*pfnParse)(openFW->fwdid, openFW->mdFilename, *vdfTree, node, level);

        argnum = 0;

        tokenizer.SetString(line);

        while(tokenizer.HasMoreTokens()) {
            if((len = strlen(tokenizer.token))) {


                // block comments
                if(tokenizer.token[0] == '/' && tokenizer.token[1] == '/')
                    break;

                // open node
                else if(!strcmp(tokenizer.token, "{")) {

                    parent = node;
                    node = (*vdfTree)->CreateNode();
                    (*vdfTree)->AppendChild(parent, node);

                    level++;
                    argnum = 0;
                    newChild = false;
                }

                // close node
                else if(!strcmp(tokenizer.token, "}")) {

                    if(level) {
                        node = node->parentNode;
                        parent = node->parentNode;
                        level--;
                        argnum = 0;
                        newChild = true;
                    }
                }

                else {
                    if(!argnum) {
                        if(newChild) {
                            tmpNode = node;
                            node = (*vdfTree)->CreateNode(parent);
                            (*vdfTree)->AppendNode(tmpNode, node);
                            newChild = false;
                        }
                        (*vdfTree)->SetKeyPair(node, tokenizer.token);
                    }
                    else {
                        (*vdfTree)->SetKeyPair(node, NULL, tokenizer.token);
                        newChild = true;
                        if(pfnParse != NULL)
                            retVal = (*pfnParse)(openFW->fwdid, openFW->mdFilename,
                                                 *vdfTree, node, level);
                    }
                    argnum++;
                }
                if(argnum == 1)
                    newChild = true;

            }
        }
    }

    if(argnum == 1 && pfnParse != NULL)
        retVal = (*pfnParse)(openFW->fwdid, openFW->mdFilename, *vdfTree, node, level);


    fclose(pFile);

    return true;

}

/**
 *  Saves a VDF file
 *
 *  @param  char*       Name of target file
 *  @param  VDFTree*    VDFTree object source
 */
bool VDFTreeParser::SaveVDF(const char *filename, VDFTree *vdfTree)
{
    FILE*   pFile;
    VDFNode *node;
    int     level;
    char    buffer[512];
    char    tabs[128];
    bool    rew;

    node     =  vdfTree->rootNode;
    level    =  0;

    if(filename == NULL || node == NULL)
        return false;

    pFile = fopen(filename, "w+");
    if(!pFile)
        return false;
    rew = false;
    *buffer = '\0';

    while(node) {
        if(rew) {
            if(level && !node->nextNode) {
                level--;
                TABFILL(tabs, level);
                sprintf(buffer, "%s}\n", tabs);
                node = node->parentNode;
                fputs(buffer, pFile);
                *buffer = '\0';
                continue;
            }
            node = node->nextNode;
            rew = false;
            continue;
        }
        if(node->key && *(node->key)) {
            TABFILL(tabs, level);
            sprintf(buffer, "%s\"%s\"", tabs, node->key);
            if(node->value && *(node->value))
                sprintf(buffer + strlen(buffer), " \"%s\"\n", node->value);
            else
                strcat(buffer, "\n");
        }
        if(node->childNode) {
            TABFILL(tabs, level);
            level++;
            node = node->childNode;
            sprintf(buffer + strlen(buffer), "%s{\n", tabs);
        }
        else
            rew = true;

        fputs(buffer, pFile);
        *buffer = '\0';
    }

    fclose(pFile);
    return true;
}


// --- VDFSearch Implementation ---

VDFSearch::VDFSearch()
{
    searchStr = NULL;
    levelAnchor = NULL;
    Reset();
}

VDFSearch::~VDFSearch()
{
    FinalizeArray(searchStr);
    FinalizeArray(levelAnchor);
}


/**
 *  Finds next node in current search
 *  @return true if found a/another node that matches search
 */
bool VDFSearch::FindNext()
{
    if(searchStr == NULL)
        return false;

    bool    mayGoDeeper;

    VDFNode *cache;
    VDFNode *anchor;

    mayGoDeeper = false;

    if(cursor == NULL) {
        cursor = searchTree->rootNode;
        if(Match(cursor))
            return true;
    }

    while(true) {

        mayGoDeeper = (searchLevel > -1) ? (currentLevel < searchLevel) : true;

        cache = cursor;

        if(cursor == NULL)
            break;

        if(mayGoDeeper && Walk(DIR_CHILD)) {
            MarkAnchor(cache);
            currentLevel++;

            if(Match(cursor))
                return true;

        }
        else {
            if(Walk(DIR_NEXT)) {
                if(Match(cursor))
                    return true;
            }
            else if(currentLevel) {

                while(currentLevel--) {

                    anchor = GetAnchorNode(currentLevel);

                    if(anchor) {
                        cursor = anchor->nextNode;
                        levelAnchor[currentLevel] = NULL;
                    }
                    if(cursor) {
                        if(Match(cursor))
                            return true;
                        break;
                    }
                }
            }
            else if((anchor = GetAnchorNode(0)))
                    cursor = anchor->nextNode;
            else
                break;
        }
    }

    return false;
}

/**
 *  Sets the correct position of a search before calling FindNext
 *  @param  refNode Search after this one.
 *  @return         Returns the node or NULL on end.
 */
VDFNode *VDFSearch::FindNextNode(VDFNode *refNode)
{
    size_t level;
    VDFNode *cache;

    if(refNode != cursor) {

        if(refNode != NULL) {

            cache = refNode;
            level = VDFTree::GetNodeLevel(refNode);
            currentLevel = level;

            while((cache = cache->parentNode)) {
                currentLevel--;
                this->MarkAnchor(cache);
            }

            // if has no parents, make sure anchor in 0 is empty
            if((size_t)currentLevel == level)
                levelAnchor[0] = NULL;

            currentLevel = level;

            cursor = refNode;

        }
        else {
            cursor = NULL;
            currentLevel = 0;
        }
    }
    if(FindNext())
        return cursor;
    return NULL;
}


/**
 *  Mark an node as a node in the 'anchors heap'.
 *  Anchor nodes are nodes marked in each level whenever search
 *  goes to a deeper level (child).
 */
void VDFSearch::MarkAnchor(VDFNode *anchor)
{
    if((size_t)currentLevel >= levelCount) {
        while((size_t)currentLevel >= levelCount) {
            GrowPArray(&levelAnchor, levelCount);
            levelCount++;
        }
    }
    levelAnchor[currentLevel] = anchor;
}


/**
 *  Gets the 'anchor' on heap on a specified level.
 *  @param  unsigned int    Target level.
 *  @return The anchor node or NULL if it's an invalid level or
 *          the slot is empty.
 */
VDFNode *VDFSearch::GetAnchorNode(UINT level)
{
    if(level > levelCount)
        return NULL;
    return levelAnchor[level];
}

/**
 *  Moves cursor node one step to a specific direction.
 *  @return true if the movement suceeded.
 */
bool VDFSearch::Walk(UINT direction)
{

    VDFNode *param;
    param = (direction == DIR_CHILD) ? cursor->childNode :
            cursor->nextNode;
    if (param == NULL)
        return false;
    cursor = param;
    return true;
}

/**
 *  Checks if a node matches with the current search.
 *  @param  VDFNode*    Node to compare.
 *  @return             true if it matches.
 */
bool VDFSearch::Match(VDFNode *matchNode)
{
    char    *param;
    char    *cache;
    bool    match;
    bool    mayCheckMatch;

    mayCheckMatch = (searchLevel > -1) ? (currentLevel == searchLevel) : true;

    if(!mayCheckMatch)
        return false;

    cache = NULL;
    param = (searchFlags & VDF_MATCH_VALUE) ? matchNode->value : matchNode->key;

    if(param == NULL)
        return false;

    if(strcmp(searchStr, "*") == 0)
        return true;

    if(searchFlags & VDF_IGNORE_CASE) {
        cache = new char[strlen(param) + 1];
        ToLowerCase(param, cache);
        param = cache;
    }

    match = (strcmp(param, searchStr) == 0);

    FinalizeArray(cache);

    return match;
}



/**
 *  Sets a new search, if there's already a search taking place, it's reset.
 *  @param  schTree     Vdf tree to look into.
 *  @param  search      Search string.
 *  @param  flags       Default search is for key case sensitive,
 *                      might be changed by setting:<br><code>
 *                      VDF_MATCH_VALUE </code><br>and<code>
 *                      VDF_IGNORE_CASE
 *  @param  level       Level in which search will be valid. When -1 (default)
 *                      all levels match.
 */
void VDFSearch::SetSearch(VDFTree *schTree, char *search, UINT flags, int level)
{
    Reset();
    searchLevel =   level;
    searchFlags =   flags;
    searchTree  =   schTree;

    const UINT startAnchors = 6;

    searchStr = new char[strlen(search) + 1];

    if(flags & VDF_IGNORE_CASE)
        ToLowerCase(search, searchStr);
    else
        strcpy(searchStr, search);

    // start with 6 levels to avoid unnecessary grows
    levelCount = startAnchors;
    levelAnchor = new VDFNode*[startAnchors];
    memset(levelAnchor, 0, startAnchors * sizeof(VDFNode*));

}

/**
 *  Resets search.
 */

void VDFSearch::Reset()
{
    FinalizeArray(searchStr);
    FinalizeArray(levelAnchor);
    searchStr   =   NULL;
    searchLevel =   -1;
    searchFlags =   VDF_MATCH_KEY;
    currentLevel = 0;
    cursor = NULL;
    levelCount = 0;
}


// --- VDFCollection implementation ---

VDFCollection::VDFCollection()
{
    treeCounter = 0;
    searchCounter = 0;
    //openForwards = 0;
    //parseForwards = 0;
    vdfTrees = NULL;

    parseForward = new ParseForward*[MAX_PARSE_FORWARDS];
    openForward = new OpenForward*[MAX_OPEN_FORWARDS];

    memset(parseForward, 0, MAX_PARSE_FORWARDS);
    memset(openForward, 0, MAX_OPEN_FORWARDS);

}

VDFCollection::~VDFCollection()
{
    Destroy();
}


/**
 *  Object freeing
 */
void VDFCollection::Destroy()
{
    size_t i;

    for(i = 0; i < treeCounter; i++) {
        if(vdfTrees[i] == NULL)
            continue;
        Finalize(vdfTrees[i]->vdfTree);
        FinalizeArray(vdfTrees[i]->vdfFile);
    }
    FinalizeArray(vdfTrees);

    for(i = 0; i < searchCounter; i++) {
        if(vdfSearch[i] == NULL)
            continue;
        Finalize(vdfSearch[i]);
    }

    FinalizeArray(vdfSearch);
    FinalizeArray(parseForward);
    FinalizeArray(openForward);

    searchCounter = 0;
    treeCounter = 0;
}

/**
 *  Looks for a free parse slot. The max number of simultaneos parsing
 *  (nested) is defined by MAX_PARSE_FORWARDS.
 *  @return     The index of an available slot or -1 if all slots are being used.
 */
int VDFCollection::GetFreeParserID()
{

    for(UINT i = 0; i < MAX_PARSE_FORWARDS; i++) {
        if(parseForward[i] == NULL)
            return i;
    }
    return -1;
}

/**
 *  Looks for a free parse slot. The max number of simultaneos parsing
 *  (nested) is defined by MAX_PARSE_FORWARDS.
 *  @return     The index of an available slot or -1 if all slots are being used.
 */
int VDFCollection::GetFreeOpenTreeID()
{

    for(UINT i = 0; i < MAX_PARSE_FORWARDS; i++) {
        if(openForward[i] == NULL)
            return i;
    }
    return -1;
}

/**
 *  Starts parsing process.
 *  @param  filename    Name of the file to be parsed.
 *  @param  pFW         Forward settings.
 */
void VDFCollection::ParseTree(const char *filename, ParseForward *pFW)
{
    VDFTreeParser parser;

    if(pFW == NULL)
        return;

    parser.ParseVDF(filename, pFW);
}


/**
 *  Adds a new vdf tree to collection
 *  @param  filename    Name of the tree file. Can be NULL if you're
 *                      creating a new Tree.
 *  @param  create      Set it to <code>true</code> if you're creating a Tree
 *                      rather than opening an existing one.
 *  @return             The VDFTree pointer or NULL on fail.
 */
VDFTree *VDFCollection::AddTree(const char *filename, bool create, OpenForward *openFW)
{
    VDFTreeParser   parser;
    VDFTree     *vdfTree;

    vdfTree  = NULL;

    if(create) {
        vdfTree = new VDFTree;
        vdfTree->CreateTree();
    }
    else {
        if(!parser.OpenVDF(filename, &vdfTree, openFW))
            return  NULL;
    }

    GrowPArray(&vdfTrees, treeCounter);

    vdfTrees[treeCounter] = new VDFEnum;
    vdfTrees[treeCounter]->vdfFile = new char[strlen(filename) + 1];
    strcpy(vdfTrees[treeCounter]->vdfFile, filename);
    vdfTrees[treeCounter]->vdfTree = vdfTree;
    vdfTree->treeId = treeCounter++;

    return vdfTree;
}

/**
 *  Adds a new Search object to collection
 *  @return A pointer to the new Search object.
 */
VDFSearch *VDFCollection::AddSearch()
{
    VDFSearch *newSearch;

    GrowPArray(&vdfSearch, searchCounter);
    newSearch = new VDFSearch;
    newSearch->searchId = searchCounter;
    vdfSearch[searchCounter++] = newSearch;

    return newSearch;
}

/**
 *  Sets a search string/type to a search pointer included with <code>AddSearch</code>.
 *  @param  search      Target search pointer.
 *  @param  tree        Tree in which search will be performed.
 *  @param  searchStr   String to be searched.
 *  @param  type        Search for key or value - VDF_MATCH_KEY or VDF_MATCH_VALUE.
 *                      <br>Note: searching for both at the same time isn't supported.
 *  @param  level       Checks which level to perform search. If -1 (default) all levels match.
 *  @param  ignoreCase  If different from 0 it doesn't match case.
 */
void VDFCollection::SetSearch(VDFSearch *search, VDFTree *tree, char *searchStr, UINT type,
                                        int level, UINT ignoreCase)
{
    UINT flags;

    if(search == NULL || tree == NULL)
        return;

    flags = type | (ignoreCase) ? VDF_IGNORE_CASE : 0;

    search->SetSearch(tree, searchStr, flags, level);
}

/**
 *  Removes a search from list
 *  param   @index  Index of the search to be removed.
 */
void VDFCollection::RemoveSearch(const UINT index)
{
    if(index < searchCounter)
        Finalize(vdfSearch[index]);
}

/**
 *  Removes a specific tree
 *  @param  index   Index of a tree.
 */
void VDFCollection::RemoveTree(const UINT index)
{
    Finalize(vdfTrees[index]);
}

/**
 *  Removes a specific tree
 *  @param  tree    Tree object to be destroyed.
 */
void VDFCollection::RemoveTree(VDFTree **tree)
{
    if(*tree != NULL) {
        vdfTrees[(*tree)->treeId] = NULL;
        (*tree)->DestroyTree();
    }
}

/**
 *  Get a contatiner by its index
 *  @param  unsigned int    Target index
 *  @return                 The container or null if it's been deleted
 *                          or if index is out of bounds.
 */
VDFEnum *VDFCollection::GetContainerById(const UINT index)
{
    if(index < treeCounter)
        return vdfTrees[index];
    return NULL;
}

