//
// Created by Rahul Kumar on 2/13/20.
//
#include "string.h"
#include "object.h"
#include "helper.h"
#include <iostream>
#pragma once

/*************************************************************************
 * Schema::
 * A schema is a description of the contents of a data frame, the schema
 * knows the number of columns and number of rows, the type of each column,
 * optionally columns and rows can be named by strings.
 * The valid types are represented by the chars 'S', 'B', 'I' and 'F'.
 */
class Schema : public Object {
 public:
	 char* column_types;
	 String** column_names;
	 String** row_names;
	 size_t column_num;
	 size_t row_num;
	 size_t column_cap;
	 size_t row_cap;

  /** Copying constructor */
  Schema(Schema& from):
	  Object(from) {
		  column_num = from.column_num;
		  row_num = from.row_num;
		  memcpy(column_names, from.column_names, column_num);
	 	  memcpy(row_names, from.row_names, row_num);
	  }
 
  /** Create an empty schema **/
  Schema() {
	  column_types = new char[10];
	  column_names = new String*[10];
	  row_names = new String*[10];
	  column_num = 0;
	  row_num = 0;
	  column_cap = 10;
	  row_cap = 10;
  }
 
  /** Create a schema from a string of types. A string that contains
    * characters other than those identifying the four type results in
    * undefined behavior. The argument is external, a nullptr argument is
    * undefined. **/
  Schema(const char* types) {
      column_types = new char[10];
      column_names = new String*[10];
      row_names = new String*[10];
      column_num = 0;
      row_num = 0;
      column_cap = 10;
      row_cap = 10;
	  for(size_t i = 0; i < strlen(types); i++) {		  
		  add_column(types[i], nullptr);
	  }
  }
 
  /** Add a column of the given type and name (can be nullptr), name
    * is external. Names are expectd to be unique, duplicates result
    * in undefined behavior. */
  void add_column(char typ, String* name) {
	  if(column_num >= column_cap) {
		  column_cap = column_cap * 2;
		  String** new_c_names = new String*[column_cap];
		  for (int i = 0; i < column_num; i++) {
                    new_c_names[i] = column_names[i];
		  }
		  delete[] column_names;
		  column_names = new_c_names;
	  }
	  if(typ == *"S") {
		  column_types[column_num] = *"S";
		  column_names[column_num] = name;
		  column_num ++;
	  }
	  else if (typ == *"B") {
                  column_types[column_num] = *"B";
                  column_names[column_num] = name;
                  column_num ++;
          }
	  else if(typ == *"I") {
                  column_types[column_num] = *"I";
                  column_names[column_num] = name;
                  column_num ++;
          }
	  else if(typ == *"F") {
                  column_types[column_num] = *"F";
                  column_names[column_num] = name;
                  column_num ++;
          }
  }
 
  /** Add a row with a name (possibly nullptr), name is external.  Names are
   *  expectd to be unique, duplicates result in undefined behavior. */
  void add_row(String* name) {
	  for(size_t i = 0; i < row_num; i++) {
		  String* cur = row_names[i];
		  if(cur->equals(name)) {
			  return;
		  }
	  }
	  if(row_num >= row_cap) {
		  row_cap = row_cap * 2;
                  String** new_r_names = new String*[row_cap];
                  for (int i = 0; i < row_num; i++) {
                    new_r_names[i] = row_names[i];
                  }
                  delete[] row_names;
                  row_names = new_r_names;
          }
	  row_names[row_num] = name;
	  row_num ++;
  }
 
  /** Return name of row at idx; nullptr indicates no name. An idx >= width
    * is undefined. */
  String* row_name(size_t idx) {
	  return row_names[idx];
  }
  /** Return name of column at idx; nullptr indicates no name given.
    *  An idx >= width is undefined.*/
  String* col_name(size_t idx) {
	  return column_names[idx];
  }

  /** Return type of column at idx. An idx >= width is undefined. */
  char col_type(size_t idx) {
	  return column_types[idx];
  }
 
  /** Given a column name return its index, or -1. */
  int col_idx(const char* name) {
	  String* s = new String(name);
	  for(int i = 0; i < column_num; i++) {
		  String* cur = column_names[i];
		  if(s->equals(cur)) {
			 return i;
		  } 
	  }
	  return -1;
  }
 
  /** Given a row name return its index, or -1. */
  int row_idx(const char* name) {
	  String* s = new String(name);
          for(int i = 0; i < row_num; i++) {
                  String* cur = row_names[i];
                  if(s->equals(cur)) {
                         return i;
                  }
          }
          return -1;
  }
 
  /** The number of columns */
  size_t width() {
	  return column_num;
  }
 
  /** The number of rows */
  size_t length() {
	  return row_num;
  }
};
