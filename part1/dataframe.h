//
// Created by Rahul Kumar on 2/13/20.
//
#include "schema.h"
#include "rower.h"
#include "column.h"
#ifndef SUBMISSION_DATAFRAME_H
#define SUBMISSION_DATAFRAME_H

#endif //SUBMISSION_DATAFRAME_H



/****************************************************************************
 * DataFrame::
 *
 * A DataFrame is table composed of columns of equal length. Each column
 * holds values of the same type (I, S, B, F). A dataframe has a schema that
 * describes it.
 */
class DataFrame : public Object {
	public:
		Column** data; //array of pointer of columns
		Schema s;      //schema of the dataframe
		size_t num;    //current number of columns 
		size_t cap;    //current capacity of the "data" field 

  /** Create a data frame with the same columns as the given df but with no rows or rownmaes */
  DataFrame(DataFrame& df):
	  Object(df) {
		  s = df.s;
		  num = df.num;
		  cap = df.cap;
		  data = new Column*[num];
		  memcpy(data, df.data, num);
	  }
  /** Create a data frame from a schema and columns. All columns are created
    * empty. */
  DataFrame(Schema& schema):
	  Object(schema) {
		  s.column_num = schema.column_num;
		  s.row_num = schema.row_num;
		  s.column_cap = schema.column_cap;
		  s.row_cap = schema.row_cap;
		  s.column_types = new char(s.column_num);
		  s.column_names = new String*[s.column_num];
		  s.row_names = new String*[s.row_num];
		  memcpy(s.column_types, schema.column_types, s.column_num);
		  memcpy(s.column_names, schema.column_names, s.column_num);
		  memcpy(s.row_names, schema.row_names, s.row_num);
		  data = new Column*[s.column_num];
	  }
  /** Returns the dataframe's schema. Modifying the schema after a dataframe
    * has been created in undefined. */
  Schema& get_schema() {
	  return s;
  }

  /** Adds a column this dataframe, updates the schema, the new column
    * is external, and appears as the last column of the dataframe, the
    * name is optional and external. A nullptr colum is undefined. */
  void add_column(Column* col, String* name) {
	  if (col != nullptr) {
		  data[s.column_num] = col;
		  s.add_column(col->get_type(), name);
	  }
  }
  /** Return the value at the given column and row. Accessing rows or
   *  columns out of bounds, or request the wrong type is undefined.*/
  int get_int(size_t col, size_t row) {
	  if(s.column_types[col] == 'I') {
		  IntColumn* i = data[col]->as_int();
		  return i->get(row);
	  }
  }

  bool get_bool(size_t col, size_t row) {
	  if(s.column_types[col] == 'B') {
                  BoolColumn* i = data[col]->as_bool();
                  return i->get(row);
          }
  }

  float get_float(size_t col, size_t row) {
	  if(s.column_types[col] == 'F') {
                  FloatColumn* i = data[col]->as_float();
                  return i->get(row);
          }
  }

  String* get_string(size_t col, size_t row) {
	  if(s.column_types[col] == 'S') {
                  StringColumn* i = data[col]->as_string();
                  return i->get(row);
          }
  }
 
  /** Return the offset of the given column name or -1 if no such col. */
  int get_col(String& col) {
	  char* c = col.cstr_;
	  return s.col_idx(c);
  }
 
  /** Return the offset of the given row name or -1 if no such row. */
  int get_row(String& col) {
	  char* c = col.cstr_;
	  return s.row_idx(c);
  }

  /** Set the value at the given column and row to the given value.
    * If the column is not  of the right type or the indices are out of
    * bound, the result is undefined. */
  void set(size_t col, size_t row, int val) {
	  if(col < s.column_num && data[col]->get_type() == 'I') {
		  if(row < s.row_num) {
			  data[col]->as_int()->set(row, val);
		  }
	  }
  }

  void set(size_t col, size_t row, bool val) {
	  if(col < s.column_num && data[col]->get_type() == 'B') {
                  if(row < s.row_num) {
                          data[col]->as_bool()->set(row, val);
                  }
          }
  }
  void set(size_t col, size_t row, float val) {
	  if(col < s.column_num && data[col]->get_type() == 'F') {
                  if(row < s.row_num) {
                          data[col]->as_float()->set(row, val);
                  }
          }
  }

  void set(size_t col, size_t row, String* val) {
	  if(col < s.column_num && data[col]->get_type() == 'S') {
                  if(row < s.row_num) {
                          data[col]->as_string()->set(row, val);
                  }
          }
  }
 
  /** Set the fields of the given row object with values from the columns at
    * the given offset.  If the row is not form the same schema as the
    * dataframe, results are undefined.
    */
  void fill_row(size_t idx, Row& row) {
      for (int i = 0; i < row.num;i++) {
          if (data[i]->get_type() == 'S') {
              data[i]->as_string()->set(idx, row.get_string(i));
          } else if (data[i]->get_type() == 'I') {
              data[i]->as_int()->set(idx, row.get_int(i));
          } else if (data[i]->get_type() == 'B') {
              data[i]->as_bool()->set(idx, row.get_bool(i));
          } else if (data[i]->get_type() == 'F') {
              data[i]->as_float()->set(idx, row.get_float(i));
          }
      }
  }

  /** Add a row at the end of this dataframe. The row is expected to have
   *  the right schema and be filled with values, otherwise undedined.  */
  void add_row(Row& row) {
      for (int i = 0; i < row.num;i++) {
          if (s.column_types[i] == 'S') {
              data[i]->as_string()->push_back(row.get_string(i));
          } else if (s.column_types[i] == 'I') {
              data[i]->as_int()->push_back(row.get_int(i));
          } else if (s.column_types[i] == 'B') {
              data[i]->as_bool()->push_back(row.get_bool(i));
          } else if (s.column_types[i] == 'F') {
              data[i]->as_float()->push_back(row.get_float(i));
          } else {
              continue;
          }
      }
  }

  /** The number of rows in the dataframe. */
  size_t nrows() {
	  return s.length();
  }
 
  /** The number of columns in the dataframe.*/
  size_t ncols() {
	  return s.width();
  }
 
  /** Visit rows in order */
  void map(Rower& r) {
      for (int i = 0; i < num;i++) {
          Row* ro = new Row(this->s);
          fill_row(i,*ro);
          r.accept(*ro);
      }
  }

  /** Create a new dataframe, constructed from rows for which the given Rower
    * returned true from its accept method. */
  DataFrame* filter(Rower& r) {
      DataFrame* df = new DataFrame(this->s);
      for (int i = 0; i < num;i++) {
          Row* ro = new Row(this->s);
          fill_row(i,*ro);
          if (r.accept(*ro) == true) {
              df->add_row(*ro);
          }
      }
      return df;
  }

  /** Print the dataframe in SoR format to standard output. */
  void print() {
	  for (size_t i = 0; i < s.column_num; i++) {
		  std::cout << s.column_names[i] << " ";
	  }
	  std::cout << ".\n";
	  for (size_t i = 0; i < s.row_num; i++) {
		  std::cout << s.row_names[i] << " ";

		  for (size_t j = 0; j < s.column_num; j++) {
			  Column* c = data[j];
			  if (c->get_type() == 'B') { 
				  std::cout << c->as_bool()->get(i) << " ";
			  }
			  else if (c->get_type() == 'I') {
                                  std::cout << c->as_int()->get(i) << " ";                      
			  }
			  else if (c->get_type() == 'S') { 
				  std::cout <<  c->as_string()->get(i) << " ";
			
			  }
  			  else if (c->get_type() == 'F') { 
				  std::cout << c->as_float()->get(i) << " ";
			
			  }
		  }
		  std::cout << ".\n";
	  }
  }
};

