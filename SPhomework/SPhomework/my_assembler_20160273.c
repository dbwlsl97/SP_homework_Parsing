/*
* ȭ�ϸ� : my_assembler_00000000.c
* ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
* �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
* ���� ������ ���Ǵ� ���ڿ� "00000000"���� �ڽ��� �й��� �����Ѵ�.
*/

/*
*
* ���α׷��� ����� �����Ѵ�.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// ���ϸ��� "00000000"�� �ڽ��� �й����� ������ ��.
#include "my_assembler_20160273.h"

/* ----------------------------------------------------------------------------------
* ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
* �Ű� : ���� ����, ����� ����
* ��ȯ : ���� = 0, ���� = < 0
* ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�.
*		   ���� �߰������� �������� �ʴ´�.
* ----------------------------------------------------------------------------------
*/
int main(int args, char *arg[])
{
	if (init_my_assembler() < 0)
	{
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
		return -1;
	}
	for (int i = 0; i < MAX_LINES; i++) { //������ ����� ���� ���ڿ� ����
		output[i] = (char *)malloc(sizeof(char) * 50);
	}
	if (assem_pass1() < 0) {
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}
	make_opcode_output("output_20160273.txt");
//	make_opcode_output(NULL);
	/*
	* ���� ������Ʈ���� ���Ǵ� �κ�
	*
	if(assem_pass2() < 0 ){
	printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n") ;
	return -1 ;
	}
	make_objectcode_output("output") ;
	*/
	return 0;
}

/* ----------------------------------------------------------------------------------
* ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�.
* �Ű� : ����
* ��ȯ : �������� = 0 , ���� �߻� = -1
* ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ�
*		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
*		   �����Ͽ���.
* ----------------------------------------------------------------------------------
*/
int init_my_assembler(void)
{
	int result;

	if ((result = init_inst_file("inst.data")) < 0)
		return -1;
	if ((result = init_input_file("input.txt")) < 0)
		return -1;
	return result;
}

/* ----------------------------------------------------------------------------------
* ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)��
*        �����ϴ� �Լ��̴�.
* �Ű� : ���� ��� ����
* ��ȯ : �������� = 0 , ���� < 0
* ���� : ���� ������� ������ �����Ӱ� �����Ѵ�. ���ô� ������ ����.
*
*	===============================================================================
*		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
*	===============================================================================
*
* ----------------------------------------------------------------------------------
*/
int init_inst_file(char *inst_file)
{
	//inst_table �����Ҵ�
	for (int i = 0; i < MAX_INST; i++) { 
		inst_table[i] = (inst *)malloc(sizeof(inst));
		inst_table[i]->instname = (char *)malloc(sizeof(char) * 10);
		inst_table[i]->format = (char *)malloc(sizeof(char) * 10);
		inst_table[i]->opcode = (char *)malloc(sizeof(char) * 10);
		inst_table[i]->num_operand = (char *)malloc(sizeof(char) * 10);
	}
	FILE * file;
	char *temp[4] = { NULL, }; //��ū�� �־���� �ӽú���
	for (int i = 0; i < 4; i++) {
		temp[i] = (char *)malloc(sizeof(char) * 10);
	}
	char * save = (char *)malloc(sizeof(char) * 10); // strtok�� Ȱ���� �� ���� ����
	char * i_str = (char *)malloc(sizeof(char) * 10);  

	int errno;

	file = fopen("inst.data", "r");
	if (file != NULL) {
		char strTemp[30];
		char *pStr;
		while (!feof(file))
		{
			pStr = fgets(strTemp, sizeof(strTemp), file); 
			strcpy(i_str, pStr); // pStr �Ѽ��� �����ϱ� ���� strtok�� ����� ���ڿ��� i_str�� ����
			
			save = strtok(i_str, "\t"); //strtok �� ���� save �� ����
			strcpy(temp[0], save);//strtok �� ���� temp[0] �� ����
			strcpy(inst_table[inst_index]->instname, temp[0]); //temp[0] = ��ɾ� �̸�

			save = strtok(NULL, "\t");
			strcpy(temp[1], save);
			strcpy(inst_table[inst_index]->format, temp[1]); // temp[1] = ��ɾ� ����

			save = strtok(NULL, "\t");
			strcpy(temp[2], save);
			strcpy(inst_table[inst_index]->opcode, temp[2]); // temp[2] = opcode

			save = strtok(NULL, "\t");
			strcpy(temp[3], save);
			strcpy(inst_table[inst_index]->num_operand, temp[3]); //temp[3] = ���۷��� ����
			save = strtok(NULL, "\t");

			inst_index++;
		}
		fclose(file);
	}
	return errno;
}

/* ----------------------------------------------------------------------------------
* ���� : ����� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ��̴�.
* �Ű� : ������� �ҽ����ϸ�
* ��ȯ : �������� = 0 , ���� < 0
* ���� : ���δ����� �����Ѵ�.
*
* ----------------------------------------------------------------------------------
*/
int init_input_file(char *input_file)
{
	for (int i = 0; i < MAX_LINES; i++) {
		input_data[i] = (char *)malloc(sizeof(char) * 50);
	}
	char *input_str = (char *)malloc(sizeof(char) * 50);
	FILE * file;
	file = fopen("input.txt", "r");
	int errno;

	if(file!=NULL){	
		char strTemp[50];
		char *pStr;
		while (!feof(file))
		{
			pStr = fgets(strTemp, sizeof(strTemp), file); //�� �� �� �߶� input_data �� ����
				strcpy(input_str, pStr);
				strcpy(input_data[line_num], input_str);
				line_num++;
		}

		fclose(file);
	}

	return errno;
}

/* ----------------------------------------------------------------------------------
* ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�.
*        �н� 1�� ���� ȣ��ȴ�.
* �Ű� : �Ľ��� ���ϴ� ���ڿ�
* ��ȯ : �������� = 0 , ���� < 0
* ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�.
* ----------------------------------------------------------------------------------
*/
int token_parsing(char *str)
{
	if (str == NULL) {
		sscanf("%s", "\0");
	}
	int count_com = 0;
	int i = token_line;
	token_table[i] = (token *)malloc(sizeof(token));
	token_table[i]->label = (char *)malloc(sizeof(char) * 10); 
	token_table[i]->operator = (char *)malloc(sizeof(char) * 30); 
	for (int j = 0; j < MAX_OPERAND; j++) {
		token_table[i]->operand[j] = (char *)malloc(sizeof(char) * 30); 
	}

	int op = 0;
	token_table[i]->comment = (char *)malloc(sizeof(char) * 30);
	char *zero = (char *)malloc(sizeof(char) * 30);
	char * save = (char *)malloc(sizeof(char) * 30); // ���� �������� strtok �� ����ϱ� ���� ����� ����
	char * token_str = (char *)malloc(sizeof(char) * 30); // str�� �����ϱ� ���� ����� ����
	char * toto = (char *)malloc(sizeof(char) * 30); //�޸��� �������� strtok �� ����ϱ� ���� ����� ����
	int token_count = 0;
	strcpy(token_str, str); // str �Ѽ��� �����ϱ� ���� strtok�� ����� ���ڿ��� token_str�� ����
	strcpy(toto, str);		// strtok�� ����� ���ڿ��� token_str�� ����
	save = strtok(token_str, "\t");		// strtok �� ����
	while (save != NULL) {				// ���� �������� 
			save = strtok(NULL, "\t");	// ��ū���� �߶��ְ�
			token_count++;				// ��ū ������ ���ϴ�.
	}
		
	if (strstr(str, ",")) {				// strstr�� �޸��� �� �ִ� ���ڿ��� ã��
		toto = strtok(toto, ",");		// strtok �� ����
		while (toto != NULL) {			// �޸��� ��������
			toto = strtok(NULL, ",");	// ��ū���� �߶��ְ�
			count_com++;				// �޸� ������ ���ϴ�
		}
	}
	tocount[token_line] = token_count;	// �� ���ڿ� �ϳ� �� ��ū ������ �����մϴ� ( tocount �迭�� output �Լ��� ���ϸ��� �� ��� )
								
	switch (token_count) {				// ��ū ������ �̿��� switch - case ��
	case 1: if (str[0] == '.') {		// ��ū ������ 1���̰�, ���ڿ� ������ . �̶�� .�� label �� �־��ݴϴ�
		strcpy(token_table[token_line]->operator, "");
		sscanf(input_data[token_line], "%s\t%s", token_table[token_line]->label, token_table[token_line]->operator);
		strcpy(output[token_line], input_data[token_line]);
	}
			else {						// �ƴ϶�� ��ū �ϳ��� operator �� �־��ݴϴ�.
				sscanf(input_data[token_line], "%s", token_table[token_line]-> operator);
				strcpy(output[token_line], input_data[token_line]);
			}
			break;
	case 2:								// ��ū ������ 2���̰�, ���ڿ� ������ . �̶�� .�� label��, ������ ��ū�� comment�� �־��ݴϴ�.
		if (strstr(input_data[token_line], ".")) {
		strcpy(token_table[token_line]->operator, "");
		sscanf(input_data[token_line], "%s\t%s\t%[^\r\n]", token_table[token_line]->label, token_table[token_line]->operator, token_table[token_line]->comment);
		strcpy(output[token_line], input_data[token_line]);
	}
			else if (str[0] == '\t') {	// ��ū ������ 2���̰�, ���ڿ� ó���� ���̶�� label�� ���ٴ� ��
				sscanf(input_data[token_line], "%s", token_table[token_line]->operator); //���� operator�� ������ 
				op = search_opcode(token_table[token_line]->operator);					
				if (op >= 0 && strstr(inst_table[op]->num_operand, "0")) {				 //inst_table�� ���۷��� ������ 0�ΰ��� Ȯ��
					sscanf(input_data[token_line], "%s\t\t%[^\r\n]", token_table[token_line]->operator, token_table[token_line]->comment);
					strcpy(output[token_line], input_data[token_line]);
				}
				else if (count_com == 3) {	// �� ���� comma �� ���� ���ڿ� ������ 3����� ,�� �ι� �Ἥ �� operand[0],[1],[2] �� �־���
					sscanf(input_data[token_line], "%s\t%[^,],%[^,],%s", token_table[token_line]->operator, token_table[token_line]->operand[0], token_table[token_line]->operand[1], token_table[token_line]->operand[2]);
					strcpy(output[token_line], input_data[token_line]);
				}
				else if (count_com == 2) {	// �� ���� comma �� ���� ���ڿ� ������ 3����� ,�� �ι� �Ἥ �� operand[0],[1],[2] �� �־���
					sscanf(input_data[token_line], "%s\t%[^,],%s",
						token_table[token_line]->operator, token_table[token_line]->operand[0], token_table[token_line]->operand[1]);
					strcpy(output[token_line], input_data[token_line]);
				}
				else{					// comma�� ����, label�� ������ operator, operand[9]�� ����
					sscanf(input_data[token_line], "%s\t%s", token_table[token_line]->operator, token_table[token_line]->operand[0]);
					strcpy(output[token_line], input_data[token_line]);
				}
			}
			else {	// ���ڿ� ó���� ���̸� label�� operator �� ����
				sscanf(input_data[token_line], "%s\t%s", token_table[token_line]->label, token_table[token_line]->operator);
				strcpy(output[token_line], input_data[token_line]);
				
			}
			break;
	case 3: if (str[0] == '\t') { // label �� ���� ��
		if (count_com == 2) {	// operand �� �޸��� ���� ��
			sscanf(input_data[token_line], "%s\t%[^,],%s\t%[^\r\n]", token_table[token_line]->operator, token_table[token_line]->operand[0], token_table[token_line]->operand[1], token_table[token_line]->comment);
			strcpy(output[token_line], input_data[token_line]);
		}
		else {					// operand �� �޸��� ���� ��
							strcpy(token_table[token_line]->operand[1], "");
			sscanf(input_data[token_line], "%s\t%s\t%[^\r\n]", token_table[token_line]->operator, token_table[token_line]->operand[0], token_table[token_line]->comment);
							sscanf(input_data[token_line], "%s %s %[^\r\n]", token_table[token_line]->operator, token_table[token_line]->operand[0], token_table[token_line]->comment);
			strcpy(output[token_line], input_data[token_line]);
		}
	}
			else {				// label �� ���� ��
				sscanf(input_data[token_line], "%s\t%s\t%s",
					token_table[token_line]->label, token_table[token_line]->operator, token_table[token_line]->operand[0]);
				strcpy(output[token_line], input_data[token_line]);
			}
			break;
	case 4: {	// label, operator, operand, comment�� �� ���� ��
		sscanf(input_data[token_line], "%s\t%s\t%s\t%[^\r\n]", token_table[token_line]->label, token_table[token_line]->operator,token_table[token_line]->operand[0], token_table[token_line]->comment);
		strcpy(output[token_line], input_data[token_line]);
	}
			break;
	default:
		return -1;
		break;
	}
	token_line++;
		return 0;
}

/* ----------------------------------------------------------------------------------
* ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�.
* �Ű� : ��ū ������ ���е� ���ڿ�
* ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0
* ���� :
*
* ----------------------------------------------------------------------------------
*/
int search_opcode(char *str)
{
	int realopcode = -1;

	for (int i = 0; i < inst_index; i++) {
		if (str[0] == '+') { //operator�� inst_table�� ��ɾ��̸��� ��
			str = str++; 
		}
		else if (!strcmp(str, inst_table[i]->instname)) { //operator �� 4������ ��(+) +�� �ǳʶٰ� �˻�
			return i;
		}
		else if(str=="\0") {
			realopcode = -1;
		}
	}
	return realopcode;
}


/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		   �н�1������..
*		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		   ���̺��� �����Ѵ�.
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
*	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	for (int i = 0; i < line_num; i++) { //line_num (��ü input.txt) ���� ��ŭ ��ū�Ľ��ϱ�
		token_parsing(input_data[i]); 
	}
	return 0;
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ ��ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 4��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*        ���� ���� 4�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char *file_name)
{
	FILE *make;
	if (file_name == NULL) // NULL�� ���� ���� ���������ʰ� ��¸�
		make = stdout;
	else {
		make = fopen(file_name, "w");
		printf("%s ������ �����Ǿ����ϴ� \n", file_name);
	}
	for (int i = 0; i < line_num; i++) {
		myopcode = search_opcode(token_table[i]->operator); //operator�� tsearch_opcode �� ���� opcode �˻�
		if (myopcode == -1) { //operator �� ���þ��� ��(��ɾ����̺� ���� ��)
			if (tocount[i] == 4) { //label , operator, operand, comment �� �ִٸ�
				strcpy(token_table[i]->comment, "");
				sprintf(input_data[i], "%s\t%s\t%s\t%s\n", token_table[i]->label, token_table[i]->operator,token_table[i]->operand[0], token_table[i]->comment);
				strcpy(output[i], input_data[i]);
			}
			else 
				strcpy(output[i], input_data[i]);
		}
		else {
			strcpy(token_table[i]->comment, "");
			switch (tocount[i]) { // �Ʊ� ��ū�Ľ��Լ�ó�� token ������ ����
			case 2: { //2���� operator , operand , opcode �־��ֱ�
				sprintf(input_data[i], "\t%s\t%s\t%s\n", token_table[i]->operator, token_table[i]->operand[0], inst_table[myopcode]->opcode);
				if (strstr(inst_table[myopcode]->num_operand, "0")) {
					sprintf(input_data[i], "\t%s\t%s\t%s\n", token_table[i]->operator,token_table[i]->comment, inst_table[myopcode]->opcode);
				}
				strcpy(output[i], input_data[i]);
			}
					break;
			case 3: { 
				if (strstr(inst_table[myopcode]->num_operand, "2")) { //3���ε� operand ������ 2���� operator , operand[0],[1] , opcode �־��ֱ�
					sprintf(input_data[i], "\t%s\t%s\,%s\t%s\n", token_table[i]->operator,token_table[i]->operand[0], token_table[i]->operand[1], inst_table[myopcode]->opcode);
				}
				else
					sprintf(input_data[i], "\t%s\t%s\t%s\n", token_table[i]->operator,token_table[i]->operand[0], inst_table[myopcode]->opcode);
				strcpy(output[i], input_data[i]);
			}
					break;
			case 4: { //4���� operator , operand , opcode �־��ֱ�
				sprintf(input_data[i], "%s\t%s\t%s\t%s\n",
					token_table[i]->label, token_table[i]->operator,token_table[i]->operand[0], inst_table[myopcode]->opcode);
				strcpy(output[i], input_data[i]);
			}
					break;
			default:
				break;
			}
		}
		fwrite(output[i], strlen(output[i]), 1, make); //make ���Ͽ� ����	
	}
	fputs("\n",make);
}

/* --------------------------------------------------------------------------------*
* ------------------------- ���� ������Ʈ���� ����� �Լ� --------------------------*
* --------------------------------------------------------------------------------*/


/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
* -----------------------------------------------------------------------------------
*/
//static int assem_pass2(void)
//{
//
//	/* add your code here */
//
//}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ object code (������Ʈ 1��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/