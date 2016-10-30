//============================================================================
// Name        : bayatpour_interpretor.cpp
// Author      : Mohammadreza Bayatpour
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;



vector<string> numeric_atoms_list;
vector<string> literal_atoms_list;
int IS_NUMERIC				= 0;
int IS_EOF 					= 0;
int ERROR 					= 0;
int numeric_atoms_num		= 0;
int literal_atoms_num		= 0;
int open_parentheses_num 	= 0;
int close_parentheses_num 	= 0;





string getNextToken () {

	char ch;
	string word = "";
	while (1) {

		if (cin.eof()) {
			IS_EOF = 1;
			if (word == "") {
				return "";
			} else {
				goto check_sanity;
			}
		}

        cin.get(ch);

		if (ch == '(') {
            if (word == "") {
			    return "(";
            } else {
                cin.seekg(-1, ios::cur);
                goto check_sanity;
            }
		} else if (ch == ')') {
            if (word == "") {
			    return ")";
            } else {
                cin.seekg(-1, ios::cur);
                goto check_sanity;
            }

		} else if (ch == ' ' || ch == '\n' || ch == '\r' || ch == 0) {
			//continue
			if (word == "") {
				continue;
			} else {
				//goto check, word is completed
				goto check_sanity;
			}

		}  else {
			word += ch;
		}


	}

check_sanity:
	if (48 <= word[0] && 57 >= word[0]) { // should contains only numbers
		for (int i = 1; i < word.length(); i++) {
			if (65 <= word[i] && 90 >= word[i]) {
				//return ERROR
				ERROR = 1;
				return word;
			}
		}
		IS_NUMERIC = 1;
	} else {
		IS_NUMERIC = 0;
	}
	return word;
}

int str_to_int (string str) {

	int temp, sum = 0, tenth = 1;
    int starter_char = 0;
    if (str[0] == 45) {/* minus sign */ 
        starter_char = 1;
    } else {
        starter_char = 0;
    }
	for (int i = str.length() - 1; i >= starter_char; i--) {
		if (48 <= str[i] && 57 >= str[i]) {
			temp 	= (str[i] - 48) * tenth;
			tenth	*= 10;
			sum 	+= temp;
		} else {
			//error
			cout << "ERROR in str to int function!!\n";
			exit (1);
		}
	}
    if (starter_char == 1) sum *= -1;
	return sum;
}

void print_stats () {

	cout << "LITERAL ATOMS: " << literal_atoms_list.size() << ", ";
	if (literal_atoms_list.size() != 0) {
		for (int i = 0; i < literal_atoms_list.size() - 1; i++) {
			cout << literal_atoms_list[i] << ", ";
		}
		cout << literal_atoms_list[literal_atoms_list.size() - 1] << endl;
	} else {
        cout << endl;
    }
	cout << "NUMERIC ATOMS: " << numeric_atoms_list.size() << ", ";
	int temp = 0, sum = 0;
	for (int i = 0; i < numeric_atoms_list.size(); i++) {
		temp = str_to_int (numeric_atoms_list[i]);
		sum += temp;
	}
	cout << sum << endl;

	cout << "OPEN PARENTHESES: " << open_parentheses_num << endl;
	cout << "CLOSING PARENTHESES: " << close_parentheses_num << endl;

}

int get_stats () {

	string token = "";
	while (1) {

		token = getNextToken();
		if (ERROR) {
			//return ERROR and print this token and exit
			cout << "ERROR: Invalid token " << token << endl;
			exit (1);
		} else if (token == "(") {
			// inceasre paranteses
			open_parentheses_num++;
		} else if (token == ")") {
			// inceasre paranteses
			close_parentheses_num++;
		} else if (token != "") {
			if (IS_NUMERIC) {
				numeric_atoms_num++;
				numeric_atoms_list.push_back(token);

			} else {
				literal_atoms_num++;
				literal_atoms_list.push_back(token);
			}
		} else {
			if (IS_EOF) {
				//print output
				//print_stats();
				return 0;
			}
		}
	}
}

string current;
void Init () {  		// get the first token

	current = getNextToken();
	if (ERROR) {
		cout << "ERROR: Invalid token " << current << endl;
		exit(1);
	}
}
string GetCurrent () { 	// does not read anything from input

	return current;
}
void MoveToNext () {

	current = getNextToken();
	if (ERROR) {
		cout << "ERROR: Invalid token " << current << endl;
		exit(1);
	}
}


int token_is_atom (string token) {

	if (token.length() > 1) {
		return 1;
	} else if (token.length() == 1) {
		if ((48 <= token[0] && token[0] <= 57) ||
				(65 <= token[0] && token[0] <= 90)) {
			return 1;
		} else {
			return 0;
		}
	} else {
		cerr << "WARNING: Zero length token\n" << endl;
		return 0;
	}

}

class list {
    public:
        node * head, * tail;
};
class list_element {
    public:
        node * elem;
        node * next, * prev;
};

list_element::list_element () {
    this->next = NULL;
    this->prev = NULL;
}

class node {

public:
    node();
	node * left, * right, * parent;
	string value;
	int is_NIL;
};

node::node () { //constructor

	this->left 	    = NULL;
	this->right 	= NULL;
	this->parent 	= NULL;
	this->value 	= "";
	this->is_NIL	= 0;

}

node * expr () {

	node * root;
	if (token_is_atom(GetCurrent())) { //consume atom
		node * atom_node = new node;
		atom_node->value = GetCurrent();
		root 			 = atom_node;
		MoveToNext();
	} else if (GetCurrent() == "(") {
		// create an empty node
		node * empty_node = new node;
		root			  = empty_node;
		MoveToNext(); //consume  "("
		while (GetCurrent() != ")") {
			empty_node->left 			= expr();
			empty_node->left->parent 	= empty_node;
			node * empty_node_2 		= new node;
			empty_node->right 			= empty_node_2;
			empty_node_2->parent 		= empty_node;
			empty_node 					= empty_node_2;
		}
		node * NIL_node		= empty_node;
		NIL_node->is_NIL	= 1;
        NIL_node->value     = "NIL";
		MoveToNext(); //consume ")"
	} else {
		printf ("ERROR:  parse error\n");
		ERROR = 1;
		root  = NULL;
		exit(1);
	}

	return root;
}

void print_binary_tree (node * root) {

	if (root->left == NULL) {
		if (root->is_NIL) {
			cout << "NIL";
		}  else {
			cout << root->value;
		}
	} else {
		cout << "(";
		print_binary_tree(root->left);
		cout << ".";
		print_binary_tree(root->right);
		cout << ")";
	}
}


void init () {

	current = getNextToken();
    if (current == "") {
        //empty file
        exit (1);

    }

}


void check_parantheses_and_sanity() {
    get_stats();
    
    if (open_parentheses_num == 0 &&
            close_parentheses_num == 0 &&
               numeric_atoms_num == 0 &&
                literal_atoms_num == 0) {

        cout << "ERROR: File is empty\n";
        exit (1);

    }

    if (open_parentheses_num != close_parentheses_num) {
        cout << "ERROR: The number of open paranteses is not equal to the number of close paranteses\n";
        exit (1);
    }

    cin.clear();
    IS_EOF = 0;
    cin.seekg (0, ios::beg);
   

}

/* helper functions for eval */

node * car (node * s) {
    
    return s->left;
}
node * cdr (node * s) {
    
    return s->right;
}
node * cons (node * s1, node * s2) {
    
    node * new_bin_tree = new node;
    new_bin_tree->left  = s1;
    new_bin_tree->right = s2;
    s1->parent          = new_bin_tree;
    s2->parent          = new_bin_tree;
    return new_bin_tree;
}
node * atom (node * s) {
    
    node * new_bin_tree = new node;
    if (car(s) == NULL) {
        new_bin_tree->value = "T"; 
    } else {
        new_bin_tree->is_NIL = 1;
        new_bin_tree->value = "NIL";
    }
    return new_bin_tree;
}
node * int_ (node * s) {
    
    node * new_bin_tree = atom(s);
    if (new_bin_tree->is_NIL == 0) {
        string str = s->value;
        int i;
        for (i = 0; i < str.length(); i++) {
            if (str[i] > 57 ||
                    str[i] < 48) {
                if (!(str[i] == 45 && i == 0)) { /* catch minus sign */
                    new_bin_tree->is_NIL = 1;
                    new_bin_tree->value = "NIL";
                }
            }
        }
    }

    return new_bin_tree;
}
node * null_ (node * s) {
    
    node * new_bin_tree = atom(s);
    if (new_bin_tree->is_NIL == 0 &&
            s->is_NIL == 1) {
        new_bin_tree->value  = "T";
    } else {
        new_bin_tree->is_NIL = 1;
        new_bin_tree->value = "NIL";
    }

    return new_bin_tree;
}
node * eq (node * s1, node * s2) {
    
    node * new_bin_tree = atom(s1);
    int is_undefined = 1;

    if (new_bin_tree->is_NIL == 0) {
        new_bin_tree = atom(s2);
        if (new_bin_tree->is_NIL == 0) {
            /*both s1 and s2 are one node*/
            is_undefined = 0;
            if (s1->value == s2->value) {
                new_bin_tree->value = "T";
            } else {
                new_bin_tree->is_NIL = 1;
                new_bin_tree->value = "NIL";
            }
        }
    }

    if (is_undefined) {
        return NULL;
    } else {
        return new_bin_tree;
    }
}
node * plus_ (node * s1, node * s2) {

    node * new_bin_tree = int_(s1);
    int is_undefined = 1;

    if (new_bin_tree->is_NIL == 0) {
        new_bin_tree = int_(s2);
        if (new_bin_tree->is_NIL == 0) {
            /*both s1 and s2 are one numberic atom node*/
            is_undefined = 0;
            long long res = str_to_int(s1->value) +
                str_to_int(s2->value);
            new_bin_tree->value = to_string(res);
        }
    }

    if (is_undefined) {
        return NULL;
    } else {
        return new_bin_tree;
    }

}
node * times_ (node * s1, node * s2) {

    node * new_bin_tree = int_(s1);
    int is_undefined = 1;

    if (new_bin_tree->is_NIL == 0) {
        new_bin_tree = int_(s2);
        if (new_bin_tree->is_NIL == 0) {
            /*both s1 and s2 are one numeric atom node*/
            is_undefined = 0;
            long long res = str_to_int(s1->value) *
                str_to_int(s2->value);
            new_bin_tree->value = to_string(res);

        }
    }
    
    if (is_undefined) {
        return NULL;
    } else {
        return new_bin_tree;
    }
}
node * minus_ (node * s1, node * s2) {

    node * new_bin_tree = int_(s1);
    int is_undefined = 1;

    if (new_bin_tree->is_NIL == 0) {
        new_bin_tree = int_(s2);
        if (new_bin_tree->is_NIL == 0) {
            /*both s1 and s2 are one numeric atom node*/
            is_undefined = 0;
            long long res = str_to_int(s1->value) -
                str_to_int(s2->value);
            new_bin_tree->value = to_string(res);

        }
    }
    
    if (is_undefined) {
        return NULL;
    } else {
        return new_bin_tree;
    }
}
node * less_ (node * s1, node * s2) {

    node * new_bin_tree = int_(s1);
    int is_undefined = 1;

    if (new_bin_tree->is_NIL == 0) {
        new_bin_tree = int_(s2);
        if (new_bin_tree->is_NIL == 0) {
            /*both s1 and s2 are one numeric atom node*/
            is_undefined = 0;
            if (str_to_int(s1->value) <
                str_to_int(s2->value)) {
                new_bin_tree->value = "T";
            } else {
                new_bin_tree->is_NIL = 1;
                new_bin_tree->value = "NIL";
            }
        }
    }
    
    if (is_undefined) {
        return NULL;
    } else {
        return new_bin_tree;
    }
}

node * greater_ (node * s1, node * s2) {

    node * new_bin_tree = int_(s1);
    int is_undefined = 1;

    if (new_bin_tree->is_NIL == 0) {
        new_bin_tree = int_(s2);
        if (new_bin_tree->is_NIL == 0) {
            /*both s1 and s2 are one numeric atom node*/
            is_undefined = 0;
            if (str_to_int(s1->value) >
                str_to_int(s2->value)) {
                new_bin_tree->value = "T";
            } else {
                new_bin_tree->is_NIL = 1;
                new_bin_tree->value = "NIL";
            }

        }
    }
    
    if (is_undefined) {
        return NULL;
    } else {
        return new_bin_tree;
    }
}


int length_expr (node * s) {

    node * temp_s = null_(s);
    if (temp_s->value == "T") {
        /* empty list */
        return 0;
    }

    return (1 + length_expr(cdr(s)));    
}

/* end of helper functions for eval */

#define PL_PROJ_DO_CALC_IN_EVAL_LENGTH_3_TREE(s, op)     \
do {                                                \
    if (length != 3) goto exit_;                    \
    node * s1_eval = eval (car(cdr(s)));            \
    if (s1_eval != NULL) {                          \
        node * s2_eval = eval (car(cdr(cdr(s))));   \
        if (s2_eval != NULL) {                      \
            new_bin_tree = op(s1_eval, s2_eval);    \
            if (new_bin_tree != NULL) {             \
                is_undefined = 0;                   \
                goto exit_;                         \
            }                                       \
        }                                           \
    }                                               \
} while (0);


#define PL_PROJ_DO_CALC_IN_EVAL_LENGTH_2_TREE(s, op)    \
do {                                                \
    if (length != 2) goto exit_;                    \
    node * s_eval = eval (car(cdr(s)));             \
    if (s_eval != NULL) {                           \
        /* s_eval is NOT undefined */               \
        new_bin_tree = op(s_eval);                  \
        if (new_bin_tree != NULL) {                 \
            is_undefined = 0;                       \
            goto exit_;                             \
        }                                           \
    }                                               \
} while (0);

void print_binary_tree_list_notation (node * root) {

    node * temp_eval = atom(root);

    if (temp_eval->value == "T") {
        if (root->is_NIL == 1) {
            cout << "NIL" << endl;
        } else {
            cout << root->value;
        }
    } else {
    /* this is NOT a one-node tree */
        cout << "(";
        while (cdr(root) != NULL) {
   
            /* it is I_(k) where k<=n */
            print_binary_tree_list_notation(car(root));
            if(cdr(cdr(root)) == NULL) {
            /* I_(n)*/
                temp_eval = null_(cdr(root));
                if (temp_eval->is_NIL == 1) {
                    /* right child of root is not a NIL */
                    temp_eval = cdr(root);
                    cout << " . " << temp_eval->value << ")";
                } else {
                    cout << ")";
                }
            } else {
                /* it is I_(k) where k<n */
                cout << " ";
            }
            root = cdr(root);
        }
                
    }
}


node * eval_p4 (node * s, node * a_list, node * d_list) {

    int is_undefined = 1;
    
    node * new_bin_tree = atom(s);
     if (new_bin_tree->is_NIL == 0) {
        /* this is a one node tree*/

        if (s->value == "T" ||
                s->value == "NIL") {
        /* eval(T) = T or eval(NIL) = NIL*/
           new_bin_tree = s;        
           is_undefined = 0;
           if (s->value == "NIL") new_bin_tree->is_NIL = 1;
        } else {
        /* eval(s) = s if int(s) = T */
            new_bin_tree = int_(s);
            if (new_bin_tree->is_NIL == 0) {
                /* is numeric atom */
                new_bin_tree = s;
                is_undefined = 0;
            }
        }

        goto exit_;

    } else {
        /* this is NOT a one node tree */
        int length   = length_expr(s);
        node * car_s = car(s);
        if (car_s->value == "COND") {
        /* car(s) = COND */
            node * temp_case = cdr(s);
            node * temp_eval = null_(temp_case);
            while (temp_eval->is_NIL == 1) {
                /* temp_case is NOT a NIL (one-node) tree */
                temp_eval = eval(car(car(temp_case)));
                if (temp_eval->value == "T") {
                    if (temp_eval->is_NIL == 1) { //assert
                        cout << "ERROR: implementation error! is NIL while it should not be\n";
                        exit(1);
                    }
                    new_bin_tree = eval(car(cdr(car(temp_case))));
                    is_undefined = 0;
                    goto exit_;           
                } else {
                    temp_case = cdr(temp_case);
                    temp_eval = null_(temp_case); 
                }
            }
    
        } else  if (car_s->value == "DEFUN") {


            /* 
             * 1. Check and confrim the correctness of definition
             * 2. Add the new function for the d-list
             * 3. Print the name of the new function
             */
            
            /* check the length */
            if (length != 4) {
                cout << "ERROR: DEFUN expr tree length needs to be exactly 4\n";
                exit(1);

            }
            /* check name that should be a literal */
            string func_name;
            new_bin_tree = atom(car(cdr(s)));
            if (new_bin_tree->is_NIL) {
                /* function name is not a literal atom */
                cout << "ERROR: DEFUN name is not a literal atom\n";
                exit(1);
            } else {
                func_name = new_bin_tree->value;
            }
                                    
            /* check if the 3rd element is a list */  //do we need it here?
            new_bin_tree = tree_is_list(car(cdr(cdr(s))));
            if (new_bin_tree->is_NIL) {
                /* 3rd element is NOT a list */
                cout << "ERROR: DEFUN 3rd element is not list\n";
                exit(1); 
            }
            
            /* add it to the def list */
            append_node(d_list, s);

            cout << "New function is defined: "  << func_name  << endl;
        
        } else

            /* car(s) ∈ { plus_, minus_, times_, less_, greater_} */
           if (car_s->value == "GREATER") {
           
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_3_TREE(s, greater_);
           
           } else if (car_s->value == "PLUS") {

                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_3_TREE(s, plus_);
           
           } else if (car_s->value == "MINUS") {
           
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_3_TREE(s, minus_);
           
           } else if (car_s->value == "TIMES") {
           
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_3_TREE(s, times_);
           
           } else if (car_s->value == "LESS") {
           
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_3_TREE(s, less_);
           } else if (car_s->value == "EQ") {
            /* car(s) = eq_ */ 
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_3_TREE(s,eq);                 // is it enough?
           } else if (car_s->value == "CONS") {
            /* car(s) = CONS */ 
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_3_TREE(s,cons);                 
           }       
            /* car(s) ∈ { ATOM, INT, NULL } */

           else if (car_s->value == "ATOM") {
           
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_2_TREE(s, atom);
           
           } else if (car_s->value == "INT") {

                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_2_TREE(s, int_);
           
           } else if (car_s->value == "NULL") {
           
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_2_TREE(s, null_);
           
           } else if (car_s->value == "CAR") {
           /* car(s) ∈ { CAR, CDR } */
                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_2_TREE(s, car);
           
           } else if (car_s->value == "CDR") {

                PL_PROJ_DO_CALC_IN_EVAL_LENGTH_2_TREE(s, cdr);

           } else if (car_s->value == "QUOTE") {
           /* here we do not evaluate the subexpression s */     
                new_bin_tree = car(cdr(s));
                is_undefined = 0;
                goto exit_;
           } else {
                
            /* catch function calls */
                string function_name    = car_s->value;
                
                /* def_function_tree  contains pointer to func expr in d_list */
                def_function_tree       = bound(d_list, func_name);
                call_function_tree      = s;

                /* check the name */
                if (called_function_tree->is_NIL) {
                    cout << "ERROR: " << func_name << " is undefined \n";
                    exit(1);
                } 
                

                    
                /* check the number of the param to be as same as already
                 * defined funcition */
                node * call_function_param_list = cdr(call_function_tree);
                int call_param_num = length_expr(call_function_param_list);

                node * def_function_param_list  = car(cdr(cdr(def_function_tree)));
                int def_param_num  = length_expr(def_function_param_list);
                
                if (call_param_num != def_param_num) {
                    cout << "ERROR: function: " << func_name << ": Invlaid arguments\n";
                    exit(1);
                }
                
                /* evaluate the param list of function call and then create
                 * association list based on the results of each param
                 * evaluation */
                
                
                /* evaluate the param list of function call */
                
                node * it = call_function_param_list;

                while (it != NULL) {
                    
                    



                    it = cdr(it);
                }



           }
        }
 
    }
    


exit_:
    if (is_undefined) {
        return NULL;
    } else {
        return new_bin_tree;
    }

}
void start() {

    node * def_list, * ass_list;
	node * root = NULL, * root_eval = NULL;
	do {
		root	= expr();
        root_eval = eval_p4(exp,ass_list,def_list);
        if (root_eval == NULL) {
            cout << "ERROR Eval call retruns undefined expression!\nExiting...\n";
            exit(1);
        } else {
            print_binary_tree_list_notation(root_eval);
            cout << endl;
        }
   	} while (!IS_EOF);
}

int main (int argc, char *argv[]) {

    check_parantheses_and_sanity();
	init();
	start();
	return EXIT_SUCCESS;
}
