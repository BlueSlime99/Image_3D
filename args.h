#ifndef __TP12__
#define __TP12__

#define NOOPTION ((option_t*)NULL);


option_t* opt_void(option_t* l, const char* kw, void (*f)());
option_t* opt_int(option_t* l, const char* kw, void (*f)(int));
option_t* opt_int(option_t* l, const char* kw, void (*f)(int));
option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));
void process_options(option_t* l, int argc, char* *argv);



#endif