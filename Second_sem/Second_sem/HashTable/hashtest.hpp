int hashtest()
{
	FILE *in, *outputfile, *inputfile; int number;

	outputfile = fopen("HashTable/out.txt", "w");
	inputfile = fopen("HashTable/in.txt", "r");

	fprintf(outputfile, "Enter testfile number (test1-4) --> ");
	fscanf(inputfile, "%d", &number);

	char* filepath = (char*)malloc(MAX_FILE_NAME*sizeof(char));
	sprintf(filepath, "HashTable/test%d.txt", number);
	in = fopen(filepath, "r");

	if (in == NULL)
		return 1;

	int tsize;
	fprintf(outputfile, "\nEnter table size --> ");
	fscanf(inputfile, "%d", &tsize);

	//Entering Data with a key from file
	int count, i, finish;
	fscanf(in, "%d", &finish);
	struct data** input = (struct data**)malloc(finish*sizeof(struct data*));
	for (i = 0; i < finish; ++i){
		input[i] = (struct data*)malloc(MaxStringSize*sizeof(struct data));
		input[i]->KEY = (char*)malloc(MaxStringSize*sizeof(char));
		fscanf(in, "%s%d", input[i]->KEY, &(input[i]->my_data));
	}
	count = i;
	//C-Strings


	//Inserting data in the table
	struct table* h = CreateTable(tsize);
	for (i = 0; i < count; ++i)
		InsertInTheTable(h, input[i]);
	fprintf(outputfile, "\nTable created and filled from test%d.", number);
	PrintTable(h, outputfile);
	//Inserting data in the table


	//Inserting in the table from keyboard
	struct data *ins = (struct data*)malloc(sizeof(struct data));
	fprintf(outputfile, "\n\nEnter data to insert:\n");
	key KEY = (key)malloc(MaxStringSize*sizeof(char));
	int data;
	fprintf(outputfile, "Enter key --> ");
	fscanf(inputfile, "%s", KEY);
	fprintf(outputfile, "\nEnter data(int) --> ");
	fscanf(inputfile, "%d", &data);

	ins->KEY = KEY;
	ins->my_data = data;
	InsertInTheTable(h, ins);

	fprintf(outputfile, "\n\n");
	PrintTable(h, outputfile);
	//Inserting in the table from keyboard


	//Finding in the table
	struct data* p;
	key Key = (key)malloc(MaxStringSize*sizeof(char));
	fprintf(outputfile, "\nEnter key to find data --> ");
	fscanf(inputfile, "%s", Key);
	p = FindInTheTable(h, Key);
	fprintf(outputfile, "Finded data --> ");
	if (p == NULL) fprintf(outputfile, "NULL");
	else fprintf(outputfile, "%d", p->my_data);
	//Finding in the table


	//Printing data by hash
	int hash;
	fprintf(outputfile, "\n\nEnter hash to print data -->");
	fscanf(inputfile, "%d", &hash);
	PrintList(h->mytable[hash], outputfile);
	//Printing data by hash


	//Deleting element from table
	key del = (key)malloc(100 * sizeof(char));
	fprintf(outputfile, "\n\nEnter deleting string --> ");
	fscanf(inputfile, "%s", del);
	DeleteFromTable(h, del, outputfile);

	fprintf(outputfile, "\n\nModified table:\n");
	PrintTable(h, outputfile);
	//Deleting element from table


	//Clearing and deleting table
	ClearTable(h);
	fprintf(outputfile, "\nTable cleared!\n");
	PrintTable(h, outputfile);
	DeleteTable(h);
	//Clearing and deleting table

	fprintf(outputfile, "\nAll good.\n");
	
	fclose(in);
	fclose(inputfile); 
	fclose(outputfile);

	printf("Everything in the hashtable is OK. Look in.txt and out.txt.\n");

	return 0;
}