#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>

//create a struct for customer
typedef struct {
    char name[21];
    char surname[31];
    char gender[2];
    char occupancy[31];
    char level_of_education[4];
    char email[50];
    char bank_account_number[13];
    char IBAN[28];
    char account_type[14];
    char currency_unit[4];
    int total_balance_available;
    char available_for_loan[20];
} Costumer;
//create swap method for converting big endian format
void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

/*This function takes a decimal number and first converts it to hexadecimal format and assigns it to the array. 
Since this number is in the last indexes of the array, it reverses the array. 
then it converts these hexadecimal elements from little middle endian to big little endian format with swap operation. 
Finally, the value in this array is converted back to the decimal value with the help of the function.*/
int DecToMiddleBigEndian(int dec)
{
    //create array for hexadecimal converting
    char* hex=(char*)malloc((8 + 1) * sizeof(char));;
    int i=0;
    int rem;
    memset(&hex[0], 0, sizeof(hex));
    for(int k=0; k<8;k++) hex[k]='0';
    hex[8]='\0';
    while(dec!=0)
    {
        rem = dec%16;
        if(rem<10)
            rem = rem+48;
        else
            rem = rem+55;
        hex[i] = rem;
        i++;
        dec = dec/16;
    }
    
    // create new char array for reverse the all element in the array for rigt usage
    char* r = (char*)malloc((8 + 1) * sizeof(char));
    r[8] = '\0';
 
    for(int i = 0; i < 8; i++) {
        r[i] = hex[8 - 1 - i];
    }
    
     //convert the little endian to big endian 	
	swap(&r[0], &r[6]);
    swap(&r[1], &r[7]);
    swap(&r[2], &r[4]);
    swap(&r[3], &r[5]);
    //convert the big endian version to decimal version
    int dec_num = strtol(r, NULL, 16);  // convert hex_str to decimal
    printf("Big Endian Version: %s\n", r);
    printf("Decimal Version: %ld\n", dec_num);
    
    //convert to decimal number to string
    char str[1024];
    snprintf(str, sizeof(str), "%d", dec_num );

    return dec_num;
}



//convert CSV file to binary file
int csvToBinary(const char *inp_file, const char *out_file){
    char row[1024];
    Costumer wCostumer, rCostumer;
    //clear all garbage value from memory for wCostumer and rCostumer
    memset(&wCostumer, 0, sizeof(Costumer));
    memset(&rCostumer, 0, sizeof(Costumer));
    //open input file and create pointer.
    FILE *input_file = fopen(inp_file, "rb");
    //chech if input file is null
    if(input_file == NULL)
    {
        puts("error opening input file!");
        exit (1);
    }
    //open output file and check if file is null
    FILE *output_file = fopen(out_file, "wb");
     if(output_file  == NULL)
    {
        puts("error opening output file!");
        exit (1);
    }
    int cnt = 0;
    //read file line by line and split with ","
    //read file to the end of file
    while (fgets(row, 1024, input_file ) != NULL) {
        //create an array for costumer info
        char arr[30][50];  
        //create char for split the line
        char *line  = row;
        char *field;
        //create index for getting costumer info from array
        int index = 0;
        cnt++;
        //skips the info at the beginning of the file
        if(cnt==1)continue;
        //split the row by "," and define the costumer info with this info 
        while ((field = strsep(&line, ",")) != NULL) {            
            //printf("element %d = %s\n", index, field);
            //put the splitting field to the 2d array
            strcpy(arr[index], field);
            //printf("%s\n", arr[index]); 
            //copy to costumer info from array with index number
            if(index==0) strcpy(wCostumer.name, arr[index]);
            else if(index==1) strcpy(wCostumer.surname, arr[index]);
            else if(index==2) strcpy(wCostumer.gender, arr[index]);
            else if(index==3) strcpy(wCostumer.occupancy, arr[index]);
            else if(index==4) strcpy(wCostumer.level_of_education, arr[index]);
            else if(index==5) strcpy(wCostumer.email, arr[index]);
            else if(index==6) strcpy(wCostumer.bank_account_number, arr[index]);
            else if(index==7) strcpy(wCostumer.IBAN, arr[index]);
            else if(index==8) strcpy(wCostumer.account_type, arr[index]);
            else if(index==9) strcpy(wCostumer.currency_unit, arr[index]);
            else if(index==10) wCostumer.total_balance_available = atof(arr[index]);
            else if(index==11) strcpy(wCostumer.available_for_loan, arr[index]);
            index++;
            
        }
        
        //write all info to the output file with binary form
        fwrite(&wCostumer, sizeof(wCostumer), 1, output_file);
        //printf("Name: %s ,Surname: %s, Gender: %c, Occupancy: %s, Education: %s, Email: %s, Bank Account Number: %s, IBAN: %s, Account Type: %s, Currency Unit: %s, Total Balance: %.2f, Available Loan: %s\n",
          //    wCostumer.name, wCostumer.surname, wCostumer.gender, wCostumer.occupancy, wCostumer.level_of_education, wCostumer.email, wCostumer.bank_account_number, wCostumer.IBAN, wCostumer.account_type, wCostumer.currency_unit, wCostumer.total_balance_available, wCostumer.available_for_loan);
    }   
    //close the input and output file
    fclose(input_file);
    fclose(output_file);
}
//convert binary file to the XML file
int binaryToXml(const char *inp_file, const char *out_file){
    //open the input file with read binary 
    FILE *input_file = fopen(inp_file, "rb");
    Costumer costumer;
    memset(&costumer, 0, sizeof(Costumer));
    
    //check if the input file is null
    if(input_file == NULL)
    {
        printf("Error opening file!");
        exit(1);
    }
    
    //fread(&rCostumer, sizeof(rCostumer), 1, input_file);
    /*
    fscanf(input_file,"Name: %s ,Surname: %s, Gender: %s, Occupancy: %s, Education: %s, Email: %s, Bank Account Number: %s, IBAN: %s, Account Type: %s, Currency Unit: %s, Total Balance: %d, Available Loan: %s\n",
              &rCostumer.name, &rCostumer.surname, &rCostumer.gender, &rCostumer.occupancy, &rCostumer.level_of_education, &rCostumer.email, &rCostumer.bank_account_number, &rCostumer.IBAN, &rCostumer.account_type, &rCostumer.currency_unit, &rCostumer.total_balance_available, &rCostumer.available_for_loan);
              */
    //read all binary file
    //if the while loop read is successful, the read is performed
    while (fread(&costumer, sizeof(costumer), 1, input_file) == 1) {
        printf("Name: %s ,Surname: %s, Gender: %s, Occupancy: %s, Education: %s, Email: %s, Bank Account Number: %s, IBAN: %s, Account Type: %s, Currency Unit: %s, Total Balance: %d, Available Loan: %s\n",
              costumer.name, costumer.surname, costumer.gender, costumer.occupancy, costumer.level_of_education, costumer.email, costumer.bank_account_number, costumer.IBAN, costumer.account_type, costumer.currency_unit, costumer.total_balance_available, costumer.available_for_loan);
              
     }
    //close the input file
    fclose(input_file);
    //create necessary variable
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr row_node=NULL, root_node = NULL, node = NULL, node1 = NULL;  /* node pointers */
    char buff[256];
    //open the input file with read binary form
    input_file = fopen(inp_file, "rb");
     //check the file if it is null
    if(input_file == NULL)
    {
        printf("Error opening binary file!");
        exit(1);
    }
    
    int row_id = 1;
    /*
     * Creates a new document, a node and set it as a root node  
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "records");
    xmlDocSetRootElement(doc, root_node);
    //read all binary file and create and XML file
    while (fread(&costumer, sizeof(costumer), 1, input_file) == 1) {
        //create row node with id attribute
        sprintf(buff, "%d", row_id);
        row_node = xmlNewChild(root_node, NULL, BAD_CAST "row", NULL);
        xmlNewProp(row_node, BAD_CAST "id", BAD_CAST buff);
        //create customer_info child for row node
        node = xmlNewChild(row_node, NULL, BAD_CAST "customer_info", NULL);
        // create children for customer info
        xmlNewChild(node, NULL, BAD_CAST "name", BAD_CAST costumer.name);
        xmlNewChild(node, NULL, BAD_CAST "surname", BAD_CAST costumer.surname);
        xmlNewChild(node, NULL, BAD_CAST "gender", BAD_CAST costumer.gender);
        xmlNewChild(node, NULL, BAD_CAST "occupancy", BAD_CAST costumer.occupancy);
        xmlNewChild(node, NULL, BAD_CAST "level_of_education", BAD_CAST costumer.level_of_education);
        xmlNewChild(node, NULL, BAD_CAST "email", BAD_CAST costumer.email);

        //create bank_account_info child     
        node = xmlNewChild(row_node, NULL, BAD_CAST "bank_account_info", NULL);
        //create new children for bank account info
        xmlNewChild(node, NULL, BAD_CAST "bank_account_number", BAD_CAST costumer.bank_account_number);
        xmlNewChild(node, NULL, BAD_CAST "IBAN", BAD_CAST costumer.IBAN);
        xmlNewChild(node, NULL, BAD_CAST "account_type", BAD_CAST costumer.account_type);
        //create total balance available child and also create currency unit attribute
        //add total balance available into buffer and write the value
        node1 = xmlNewChild(node, NULL, BAD_CAST "TotalBalanceAvailable", BAD_CAST buff); 
        //create currency unit attribute 
        xmlNewProp(node1, BAD_CAST "currency_unit", BAD_CAST costumer.currency_unit);
        snprintf(buff, sizeof(buff), "%d", costumer.total_balance_available);
        xmlNodeSetContent(node1, BAD_CAST buff);
        //create big endian version attribute
        snprintf(buff, sizeof(buff), "%d", DecToMiddleBigEndian(costumer.total_balance_available) );
        xmlNewProp(node1, BAD_CAST "bigEnd_Version", BAD_CAST buff);
        //create new child to the bank account info             
        xmlNewChild(node, NULL, BAD_CAST "AvailableForLoan", BAD_CAST costumer.available_for_loan);
             
        row_id++;
    }
    //save the xml file with UTF-8 form and arrange the writing
    xmlSaveFormatFileEnc(out_file, doc, "UTF-8", 1);
	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
    //xmlSaveFormatFile
    /*free the document */
    xmlFreeDoc(doc);
    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
    //xmlMemoryDump();
    fclose(input_file);
    
}

int xmlValidation(const char *inp_file, const char *out_file){
    xmlDocPtr doc;
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;
	
    const char *XMLFileName = inp_file; // write your xml file here
    const char *XSDFileName = out_file; // write your xsd file here
    
    
    xmlLineNumbersDefault(1); //set line numbers, 0> no substitution, 1>substitution
    ctxt = xmlSchemaNewParserCtxt(XSDFileName); //create an xml schemas parse context
    schema = xmlSchemaParse(ctxt); //parse a schema definition resource and build an internal XML schema
    xmlSchemaFreeParserCtxt(ctxt); //free the resources associated to the schema parser context
    
    doc = xmlReadFile(XMLFileName, NULL, 0); //parse an XML file
    if (doc == NULL)
    {
        fprintf(stderr, "Could not parse %s\n", XMLFileName);
    }
    else
    {
        xmlSchemaValidCtxtPtr ctxt;  //structure xmlSchemaValidCtxt, not public by API
        int ret;
        
        ctxt = xmlSchemaNewValidCtxt(schema); //create an xml schemas validation context 
        ret = xmlSchemaValidateDoc(ctxt, doc); //validate a document tree in memory
        if (ret == 0) //validated
        {
            printf("%s validates\n", XMLFileName);
        }
        else if (ret > 0) //positive error code number
        {
            printf("%s fails to validate\n", XMLFileName);
        }
        else //internal or API error
        {
            printf("%s validation generated an internal error\n", XMLFileName);
        }
        xmlSchemaFreeValidCtxt(ctxt); //free the resources associated to the schema validation context
        xmlFreeDoc(doc);
    }
    // free the resource
    if(schema != NULL)
        xmlSchemaFree(schema); //deallocate a schema structure

    xmlSchemaCleanupTypes(); //cleanup the default xml schemas types library
    xmlCleanupParser(); //cleans memory allocated by the library itself 
    xmlMemoryDump(); //memory dump
    
}

int main(int argc, char *argv[]){
    //get the argument from terminal
    const char *input_file = argv[1];
    const char *output_file = argv[2];
    
    //convert char to int for determine the type
    int type = atoi(argv[3]);
    //calls functions based on their type
    switch(type){
        case 1:
            csvToBinary(input_file,output_file);
            break;
        case 2:
            binaryToXml(input_file,output_file);
            break;
        case 3:
            xmlValidation(input_file,output_file);
            break;
    }
    return 0;
}







