#ifndef __VDFPARSER_H__
#define __VDFPARSER_H__

#include <stdlib.h>

#define MAX_LINE_SIZE 512
#define MAX_OPEN_FORWARDS 8
#define MAX_PARSE_FORWARDS 8

// flags for search class
#define VDF_MATCH_KEY			0
#define VDF_MATCH_VALUE			1
#define VDF_IGNORE_CASE			(1 << 1)

typedef unsigned int UINT;

void ToLowerCase(char *src, char *dest);

enum
{
	VDF_MOVEPOS_AFTER = 0,
	VDF_MOVEPOS_BEFORE,
};

enum
{
	RETURN_VDFPARSER_CONTINUE = 0,
	RETURN_VDFPARSER_STOP
};

enum
{
	RETURN_TREEPARSER_CONTINUE = 0,
	RETURN_TREEPARSER_SILENT,
	RETURN_TREEPARSER_BREAK
};

/**
 *  Simple node structure
 */
struct VDFNode
{
	VDFNode		*nextNode;
	VDFNode		*childNode;
	VDFNode		*parentNode;
	VDFNode		*previousNode;
	UINT		index;
	char		*key;
	char		*value;
};


/**
 *  VDF tree handling.
 */
class VDFTree
{
public:
					VDFTree			 ();
					~VDFTree		 ();
	void			CreateTree		 ();
	void			DestroyTree		 ();
	size_t			GetLength		 ();
	VDFNode			*CreateNode		 (VDFNode *parentNode = NULL);
	VDFNode			*GetNodeById	 (UINT id);
	void			DeleteNode		 (UINT index);
	void			DeleteNode		 (VDFNode *Node);

	static void		AppendNode		 (VDFNode *Node, VDFNode *newNode);
	static void		AppendChild		 (VDFNode *Node, VDFNode *childNode);
	static void		SetKeyPair	     (VDFNode *Node, const char *key = NULL, const char *value = NULL);
	void			SortBranchNodes	 (VDFNode *refNode, bool byKey = true, bool byNumber = false);
	static VDFNode	*GetRootNode	 (VDFNode *Node);
	static VDFNode	*GetLastNode	 (VDFNode *Node);
	static VDFNode	*GetPreviousNode (VDFNode *Node);
	static VDFNode	*GetFirstNode    (VDFNode *Node);
	static VDFNode	*GetNextNode     (VDFNode *Node);
	static size_t	CountBranchNodes (VDFNode *refNode);
	static size_t	GetNodeLevel	 (VDFNode *Node);
	void			MoveToBranch	 (VDFNode *anchorNode, VDFNode *moveNode, UINT position);
	void			MoveAsChild		 (VDFNode *parentNode, VDFNode *moveNode);

protected:
	inline bool		IsTreeNode		(VDFNode *node);

public:
	VDFNode		*rootNode;
	size_t		nodeCount;
	UINT		treeId;

protected:
	VDFNode		**nodeIndex;
};


/**
 *  Tokenizer for parsing vdf files
 */
class VDFTokenizer
{
public:
		  VDFTokenizer	();
		  ~VDFTokenizer	();
	bool  HasMoreTokens	();
	void  SetString		(char *str);

public:
	char  *token;

private:
	size_t pos;
	char  *source;
};

typedef int (*PFN_VDFOPEN)			(int fwid, const char* filename, VDFTree* tree,
									 VDFNode* node, int level);

typedef int (*PFN_VDFPARSE_KPAIR)	(int fwid, const char *filename,
									 const char* key,
									 const char* value,
									 int level);

typedef int (*PFN_VDFPARSE_BOF)		(int fwid, const char *filename);
typedef int (*PFN_VDFPARSE_EOF)		(int fwid, const char *filename);

/**
 *	Container for parse forwards
 */
struct ParseForward
{
	int fwidParser;
	int fwidStart;
	int	fwidEnd;
	char *mdFilename;
	PFN_VDFPARSE_KPAIR	pfnParser;
	PFN_VDFPARSE_BOF	pfnStart;
	PFN_VDFPARSE_EOF	pfnEnd;
};

/**
 *	Container for open forwards
 */
struct OpenForward
{
	int fwdid;
	char *mdFilename;
	PFN_VDFOPEN pfnOpen;
};

/**
 *  VDF read and write
 */
class VDFTreeParser
{
public:
		 VDFTreeParser	();
		 ~VDFTreeParser ();
	static bool ParseVDF 	(const char *filename, ParseForward *parseFW = NULL);
	static bool OpenVDF 	(const char *filename, VDFTree **vdfTree, OpenForward *openFW = NULL);
	static bool SaveVDF 	(const char *filename, VDFTree *vdfTree);
};


/**
 *	VDF Searching
 */
enum VEC_DIR
{
	DIR_CHILD = 0,
	DIR_NEXT
};

class VDFSearch
{
public:
				VDFSearch		();
				~VDFSearch		();
	bool		FindNext		();
	VDFNode		*FindNextNode	(VDFNode *refNode);
	void		SetSearch		(VDFTree *inTree, char *search,
								 UINT flags = VDF_MATCH_KEY, int level = -1);
	void		Reset			();

protected:
	bool		Match			(VDFNode *matchNode);
	bool		Walk			(UINT index);
	void		MarkAnchor		(VDFNode *anchor);
	VDFNode		*GetAnchorNode	(UINT index);

public:
	VDFNode		*cursor;
	UINT		searchId;
	int			currentLevel;
protected:
	int 		searchLevel;
	UINT		searchFlags;
	char		*searchStr;
	VDFTree		*searchTree;
	VDFNode		**levelAnchor;
	size_t		levelCount;

};


/**
 *  Container type for handling collection of trees.
 */
struct VDFEnum
{
	VDFTree *vdfTree;
	char	*vdfFile;
};


/**
 *  This class is designed to handle all trees, searches in a plugin session.
 *	It implements convenient methods to store and destroy trees and searches.
 */
class VDFCollection
{
public:
	int			GetFreeParserID 	();
	int			GetFreeOpenTreeID	();
	void		ParseTree			(const char *filename, ParseForward *parseForward);

	VDFTree		*AddTree			(const char *filename, bool create = false, OpenForward *openForward = NULL);
	VDFSearch	*AddSearch			();
	void		SetSearch			(VDFSearch *search,VDFTree *tree, char *searchStr,
									 UINT type, int level = -1, UINT ignoreCase = 0);

	void		RemoveTree			(const UINT index);
	void		RemoveTree			(VDFTree **tree);
	void		RemoveSearch		(const UINT index);
	VDFEnum		*GetContainerById	(const UINT index);

	void		killOpenForward		(int fwid);
	void		killParseForward	(int fwid);

	void		Destroy();
				VDFCollection ();
				~VDFCollection();

	size_t		treeCounter;
	VDFEnum		**vdfTrees;
	size_t		searchCounter;
	VDFSearch	**vdfSearch;

	OpenForward		**openForward;
	ParseForward	**parseForward;

};



#endif //__VDFPARSER_H__
