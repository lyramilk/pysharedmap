#ifndef _pylyramilk_tree_h_
#include "pynative.h"

#include "avltree.h"
#include "rbtree.h"
#include "stringbox.h"

#include <sys/shm.h>


unsigned int inline bkdr(const char* p,std::size_t l)
{
	unsigned int r = 0;
	for(;l > 0;--l){
		r = r*131 + (*p++);
	}
	return r;
}


class sharememory
{
	int shmid;
	char* p;

	unsigned long _size;
  public:
	sharememory()
	{
		shmid = -1;
		p = NULL;
	}

	bool init(key_t key, size_t size, int shmflg)
	{
		if(p){
			clear();
		}
		shmid = shmget(key,size,shmflg);
		if(shmid == -1){
			return false;
		}

		_size = size;
		return true;
	}

	void clear()
	{
		if(p) shmdt(p);
		p = NULL;
	}

	bool del()
	{
		if(shmid){
			shmctl(shmid,IPC_RMID,NULL);
			return true;
		}
		return false;
	}

	char* ptr()
	{
		if (p) return p;
		if(shmid != -1){
			p = (char*)shmat(shmid,0,0);
		}
		return p;
	}

	bool good()
	{
		return shmid != -1;
	}

	~sharememory()
	{
		clear();
	}
};

key_t inline strtokey(const char* sname,unsigned long lname)
{
	std::string sharedmemoryname = "lyramilk.sharedmemory.";
	sharedmemoryname.append(sname,lname);
	key_t key = bkdr(sharedmemoryname.c_str(),sharedmemoryname.size());
	return key;
}


struct boxdata
{
	unsigned long ikey;
	unsigned long ival;
};

class tree_compare: public lyramilk::bintree::datacompare
{
  public:
	lyramilk::data::stringbox_base* sb;
	const char *sv;
	unsigned long lv;
	tree_compare()
	{
	}

	virtual ~tree_compare()
	{
		
	}

	virtual int compare(const void* a,const void* b)
	{
		const char *s1 = NULL,*s2 = NULL;
		unsigned long l1 = 0,l2 = 0;

		unsigned long tmp;
		if((unsigned long)a == (unsigned long)-1l){
			s1 = sv;
			l1 = lv;
		}else{
			boxdata *d1;
			sb->get((unsigned long)a,(const char**)&d1,&tmp);
			sb->get(d1->ikey,&s1,&l1);
		}

		if((unsigned long)b == (unsigned long)-1l){
			s2 = sv;
			l2 = lv;
		}else{
			boxdata *d2;
			sb->get((unsigned long)b,(const char**)&d2,&tmp);
			sb->get(d2->ikey,&s2,&l2);
		}


		unsigned long l3 = l1;
		if(l3 > l2) l3 = l2;
		int r = memcmp((void*)s1,s2,l3);
		if (r == 0){
			if(l1 > l2) r = 1;
			if(l1 < l2) r = -1;
		}
		return r;
	}
};



struct rbtree_object
{
	lyramilk::bintree::rbtree* rbt;
	lyramilk::data::stringbox* sb;
	tree_compare* avp;

	rbtree_object()
	{
		rbt = NULL;
		sb = NULL;
		avp = NULL;
	};

	~rbtree_object()
	{
	};
};



PyObject * py_rbtree_set(PyObject *self, PyObject *args);
PyObject * py_rbtree_get(PyObject *self, PyObject *args);
PyObject * py_rbtree_share(PyObject *self, PyObject *args, PyObject *kwds);
PyObject * py_rbtree_dump(PyObject *self, PyObject *args);
PyObject * py_rbtree_static_remove(PyObject *self, PyObject *args);
PyObject * py_rbtree_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
void py_rbtree_dealloc(PyObject* self);

extern PyMethodDef pyrbtreeClassMethods[];
extern PyTypeObject rbtree_ObjectType;





struct shareabledict_object
{
	lyramilk::bintree::avltree* avt;
	lyramilk::data::stringbox_reader* sbr;
	tree_compare* avp;

	std::string sharedmemoryname;
	key_t key;
	sharememory sm;

	shareabledict_object()
	{
		avt = NULL;
		sbr = NULL;
		avp = NULL;
	};

	~shareabledict_object()
	{
	};
};

PyObject * py_shareabledict_get(PyObject *self, PyObject *args);
PyObject * py_shareabledict_new(PyTypeObject *type, PyObject *args, PyObject *kwds);
void py_shareabledict_dealloc(PyObject* self);

extern PyMethodDef pyshareabledictObjectClassMethods[];
extern PyTypeObject shareabledict_ObjectType;

#endif
