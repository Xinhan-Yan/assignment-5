//
// Created by Rahul Kumar on 2/13/20.
//
#pragma once
#include "schema.h"
#include "column.h"
#include "math.h"

#ifndef SUBMISSION_ROWER_H
#define SUBMISSION_ROWER_H

#endif //SUBMISSION_ROWER_H

/*****************************************************************************
 * Fielder::
 * A field vistor invoked by Row.
 */
class Fielder : public Object {
public:

    /** Called before visiting a row, the argument is the row offset in the
      dataframe. */
    virtual void start(size_t r);

    /** Called for fields of the argument's type with the value of the field. */
    virtual void accept(bool b);
    virtual void accept(float f);
    virtual void accept(int i);
    virtual void accept(String* s);

    /** Called when all fields have been seen. */
    virtual void done();
};


/*************************************************************************
 * Row::
 *
 * This class represents a single row of data constructed according to a
 * dataframe's schema. The purpose of this class is to make it easier to add
 * read/write complete rows. Internally a dataframe hold data in columns.
 * Rows have pointer equality.
 */
class Row : public Object {
 public:
	 Schema* s;
	 Column** data;
	 char* types;
	 size_t num;
	 size_t index;
	
  /** Build a row following a schema. */
  Row(Schema& scm) {
	  s = new Schema(scm.column_types);
	  types = scm.column_types;
	  num = scm.column_num;
	  data = new Column*[num];
	  for(size_t i = 0; i < num; i++) {
		  if (types[i] == 'S') {
			  data[i] = new StringColumn();
		  }
		  else if (types[i] == 'I') {
                          data[i] = new IntColumn();
                  }
		  else if (types[i] == 'B') {
                          data[i] = new BoolColumn();
                  }
		  else if (types[i] == 'F') {
                          data[i] = new FloatColumn();
                  }
	  }
  }
 
  /** Setters: set the given column with the given value. Setting a column with
    * a value of the wrong type is undefined. */
  void set(size_t col, int val) {
	  if(types[col] == 'I') {
		  data[col]->as_int()->set(0, val);
	  }
  }

  void set(size_t col, float val) {
	  if(types[col] == 'F') {
          data[col]->as_float()->set(0, val);
          }
  }

  void set(size_t col, bool val) {
	  if(types[col] == 'B') {
          data[col]->as_bool()->set(0, val);
          }
  }

  /** The string is external. */
  void set(size_t col, String* val) {
	  if(types[col] == 'S') {
          data[col]->as_string()->set(0, val);
      }
  }
 
  /** Set/get the index of this row (ie. its position in the dataframe. This is
   *  only used for informational purposes, unused otherwise */
  void set_idx(size_t idx) {
	  index = idx;
  }

  size_t get_idx() {
	  return index;
  }
 
  /** Getters: get the value at the given column. If the column is not
    * of the requested type, the result is undefined. */
  int get_int(size_t col) {
	  if(types[col] == 'I') {
		  return data[col]->as_int()->get(0);
          }
  }

  bool get_bool(size_t col) {
	  if(types[col] == 'B') {
                  return data[col]->as_bool()->get(0);
          }
  }

  float get_float(size_t col) {
	  if(types[col] == 'F') {
                  return data[col]->as_float()->get(0);
          }
  }

  String* get_string(size_t col) {
	  if(types[col] == 'S') {
                  return data[col]->as_string()->get(0);
          }
  }
 
  /** Number of fields in the row. */
  size_t width() {
	  return num;
  }
 
   /** Type of the field at the given position. An idx >= width is  undefined. */
  char col_type(size_t idx) {
	  return types[idx];
  }

  /** Given a Fielder, visit every field of this row. The first argument is
    * index of the row in the dataframe.
    * Calling this method before the row's fields have been set is undefined. */
  void visit(size_t idx, Fielder& f) {
      f.start(idx);
      for (int i = 0; i < num;i++) {
          if (data[i]->get_type() == 'S') {
              f.accept(data[i]);
          } else if (data[i]->get_type() == 'B') {
              f.accept(data[i]);
          } else if (data[i]->get_type() == 'F') {
              f.accept(data[i]);
          } else if (data[i]->get_type() == 'I') {
              f.accept(data[i]);
          }
      }
      f.done();
  }
 
};

/*******************************************************************************
 *  Rower::
 *  An interface for iterating through each row of a data frame. The intent
 *  is that this class should subclassed and the accept() method be given
 *  a meaningful implementation. Rowers can be cloned for parallel execution.
 */
class Rower : public Object {
public:
    /** This method is called once per row. The row object is on loan and
        should not be retained as it is likely going to be reused in the next
        call. The return value is used in filters to indicate that a row
        should be kept. */
    virtual bool accept(Row& r) {
	    return false;
    }

    /** Once traversal of the data frame is complete the rowers that were
        split off will be joined.  There will be one join per split. The
        original object will be the last to be called join on. The join method
        is reponsible for cleaning up memory. */
    void join_delete(Rower* other);
};



class RCG : public Rower {
        public:
		//schema "SSSSSSSSSS"
                virtual bool accept(Row& r) {
			String* s;
			for(int i = 0; i < 9; i++) {
				char* buffer = (char*) malloc(2);
				buffer[0] = rand()%26+'a';
				s = new String(buffer);
				r.set(i, s);
				free(buffer);
			}
		        char* buffer = (char*) malloc(10);
			for(int i = 0; i < 9; i++) {
				buffer[i] = r.get_string(i)->at(0);
			}
			s = new String(buffer);
			r.set(10, s);
			free(buffer);
			return true;
                   }
};


class Fibonacci : public Rower {
	public:
	//schema "IIIIIIIIII"
 		int t;
		bool b = true;
		virtual bool accept(Row& r) {
			for(int i = 2; i < 10; i++) {
				t = r.get_int(i-2) * r.get_int(i-1);
				if(r.get_int(i) != t) {
					b = false;
					r.set(i, t);
				}
			}
			return b;
		}
};

