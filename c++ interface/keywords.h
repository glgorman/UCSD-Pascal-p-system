
#include "text_object.h"

class key_word
{
public:
	language identify (char *ascii);
	void pToName (char *result, language aType);

	bool is_command (char *isa);
	bool is_numeric (char *isa);
	bool is_opcode (char *isa);
	bool is_delimiter (char *isa);
	bool is_seperator (char *isa);

private:
	bool is_whitespace (char *isa);
	bool is_type (char *isa, char **theTypeList);
	bool is_conditional (char *isa);
	bool is_function (char *isa);
	bool is_symbol (char *isa);
	bool is_control (char *isa);

private:
	bool is_pascal (char *isa);
	bool is_basic (char *isa);
	bool is_c (char *isa);
	bool is_lisp (char *isa);
};
