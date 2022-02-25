
int main(int argc, char *argv[]);

/**
 * @brief Infer files to process from commandline arguments
 * 
 * @param argc 
 * @param argv 
 * @param files a pointer which will be populated with the files names
 */
void infer_files(int argc, char *argv[], char *files[]);

/**
 * @brief Run full assembler process on file
 * 
 * @param path path to file
 * @return bool success value
 */
bool process_file(char *path);

bool create_output_files();