#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>


FILE* fp;

struct tempRecord
{
	char item1[30];
	char item2[30];
	char supCode[5];
	char donor[20];
	int shipnum;
	float quan;
}dist[100];

struct distribution {
	char supCode[5];
	float quan;
};

struct RECORD
{
	char item1[30];
	char item2[30];
	char supCode[5];
	char donor[20];
	int shipnum;
	float quan;
}rec[100];

void addDonation();
void create();
void display(struct tempRecord tempRec[]);
void displayRecords();
void distributeDonation();
void listDistribution();
void searchRecords();
void update();
void updateDistributionList(char supCode[], float quantity);
void writeStructToFile(int numberOfRecords, const char* fileName, const char* fileOperationMode);

int main()
{
	int ch;

	do {
		printf("\n\t\t----------MALAYSIA RED CRESCENT SOCIETY----------\n");
		printf("\n\t\t-----COVID-19 INVENTORY MANAGEMENT SYSTEM-----");
		printf("\n1. Create records");
		printf("\n2. Update records");
		printf("\n3. Search records");
		printf("\n4. Display records");
		printf("\n5. List distribution");
		printf("\n6. Exit system");


		printf("\nSelect an option: ");
		scanf("%d", &ch); while (getchar() != '\n');

		switch (ch) {
		case 1:
			create();
			break;
		case 2:
			update();
			break;
		case 3:
			searchRecords();
			break;
		case 4:
			displayRecords();
			break;
		case 5:
			listDistribution();
			break;
		case 6:
			printf("\n\t\t Bye, quitting the system!");
			break;

		default:
			printf("\n\t\t Invalid choice! Try again");
		}


	} while (ch != 6);
	return 0;
}

void create() {
	FILE* fp;

	fp = fopen("donation.txt", "a+");
	char ch;
	int con;
	static int ctr = 0;
	do
	{
		printf("\n\t\t---Please enter name of item word by word (This is internal system error, it is being updated).---");
		printf("\n\n\t Enter first name of item: ");
		fgets(rec[ctr].item1, sizeof(rec[ctr].item1), stdin);
		printf("\n\n\t Enter second name of item: ");
		fgets(rec[ctr].item2, sizeof(rec[ctr].item2), stdin);
		
		printf("\n\t Enter supply code: ");
		fgets(rec[ctr].supCode, sizeof(rec[ctr].supCode), stdin);

		printf("\n\t Enter name of donating country: ");
		fgets(rec[ctr].donor, sizeof(rec[ctr].donor), stdin);

		printf("\n\t Enter number of shipment: ");
		scanf("%d", &rec[ctr].shipnum);

		printf("\n\t Enter quantity of items: ");
		scanf("%f", &rec[ctr].quan);

		fprintf(fp, "%s%s%s%s%d %.2f\n\n", rec[ctr].item1, rec[ctr].item2, rec[ctr].supCode, rec[ctr].donor, rec[ctr].shipnum, rec[ctr].quan);
	
		printf("\n\t Another record?(Y/N): ");
		scanf("%c", &ch);
		if (ch == 'y') printf("%c", ch);
		con = getchar();
		while (getchar() != '\n');
		if (con == 'y' || con == 'Y')
			fputc('\n', fp);
		ctr++;
		if (ctr > 4)
			printf("\n\tAll records have been entered.");
	} while ((con == 'y' || con == 'Y') && ctr < 5);
	fclose(fp);
}

void update() {
	int opt;
	printf("\n\t1. Add donation.\n\t2. Make a distribution.");
	printf("\nSelect Option (1/2): ");
	scanf("%d", &opt);
	switch (opt) {
	case 1:
		addDonation();
		break;
	case 2:
		distributeDonation();
		break;
	default:
		printf("\tInvalid choice! Try again.");
		return;
	}
}

void addDonation() {
	fp = fopen("donation.txt", "r");
	char donationCode[5];
	float amount;
	int i = 0;
	printf("\nEnter code you are looking for: ");
	scanf("%s", &donationCode);
	printf("\nEnter amount: ");
	scanf("%f", &amount);


	while (!feof(fp))
	{
		int ret = fscanf(fp, "%s%s%s%s%d%f", &rec[i].item1, &rec[i].item2, &rec[i].supCode, &rec[i].donor,
			&rec[i].shipnum, &rec[i].quan);

		if (ret == -1) break;
		else {
			int value = strcmp(donationCode, rec[i].supCode);
			if (value == 0) {
				rec[i].quan = rec[i].quan + amount;
				rec[i].shipnum = rec[i].shipnum + 1;
			}
			
			++i;
		}
	}
	fclose(fp);
	writeStructToFile(i, "donation.txt", "w");
}

void distributeDonation() {
	fp = fopen("donation.txt", "r");
	char donationCode[5];
	float amount;
	int i = 0;
	printf("\nEnter code you are looking for: ");
	scanf("%s", &donationCode);
	printf("\nEnter amount: ");
	scanf("%f", &amount);

	while (!feof(fp))
	{
		int ret = fscanf(fp, "%s%s%s%s%d%f", &rec[i].item1, &rec[i].item2, &rec[i].supCode, &rec[i].donor,
			&rec[i].shipnum, &rec[i].quan);

		if (ret == -1) break;
		else {

			int value = strcmp(donationCode, rec[i].supCode);
			if (value == 0) {
				if (rec[i].quan < amount) {
					printf("Insufficient items in inventory!");
					return;
				}
				rec[i].quan = rec[i].quan - amount;
				updateDistributionList(donationCode, amount);
			}

			++i;
		}
	}
	fclose(fp);
	writeStructToFile(i, "donation.txt", "w");

} 



void searchRecords()
{
	char donationCode[5];
	bool itemFound = false;
	struct tempRecord inventoryRecord[1];
	fp = fopen("donation.txt", "r");
	if (fp == NULL)
	{
		printf("File does not exist.");
		return;
	}
	else
	{
		printf("\nEnter Donation Code:");
		scanf("%s", &donationCode);

		rewind(fp);
		while (!feof(fp))
		{
			int ret = fscanf(fp, "%s%s%s%s%d%f", &inventoryRecord[0].item1, &inventoryRecord[0].item2, &inventoryRecord[0].supCode, &inventoryRecord[0].donor,
				&inventoryRecord[0].shipnum, &inventoryRecord[0].quan);

			if (ret == -1) {
				break;

			}
			else {
				int value = strcmp(donationCode, inventoryRecord[0].supCode);
				if (value == 0) {
					printf("\nItem found!\n");
					display(inventoryRecord);
					itemFound = true;
				}

			}
		}

		if (itemFound = false)
			printf("Item Not found for Donation Code : %s ", donationCode);

	}
	fclose(fp);
}

void display(struct tempRecord tempRec[])
{
	int ctr = 0;
	printf("\n\n\tItem name:%-20s%s\t\tCode:%-3s\tDonor:%-10s\tShipment number:%d\tQuantity:%.2f\n",
		tempRec[ctr].item1, tempRec[ctr].item2, tempRec[ctr].supCode, tempRec[ctr].donor, tempRec[ctr].shipnum, tempRec[ctr].quan);


}

void displayRecords()
{
	struct tempRecord inventoryRecord[1];
	fp = fopen("donation.txt", "r");
	if (fp == NULL)
	{
		printf("File does not exist.");

	}
	else
	{
		printf("\tFile Records Reading.....");

		rewind(fp);
		while (!feof(fp))
		{
			int ret = fscanf(fp, "%s%s%s%s%d%f", &inventoryRecord[0].item1, &inventoryRecord[0].item2, &inventoryRecord[0].supCode, &inventoryRecord[0].donor,
				&inventoryRecord[0].shipnum, &inventoryRecord[0].quan);

			if (ret == -1) {
				break;
			}
			else
				display(inventoryRecord);
			
		}

	}
	fclose(fp);
}

void updateDistributionList(char supCode[], float quantity)
{
	FILE* fp;
	fp = fopen("dist.txt", "a");
	fprintf(fp, "%s\t%.2f\n", supCode, quantity);
	fclose(fp);
}

void writeStructToFile(int numberOfRecords, const char* fileName, const char* fileOperationMode)
{
	FILE* fp;
	fp = fopen(fileName, fileOperationMode);
	int i = 0;
	printf("TOTAL RECORDS: %d\n", numberOfRecords);

	for (i = 0; i < numberOfRecords; ++i)
	{
		fprintf(fp, "%s %s\t%s\t%s\t%d\t%.2f\n", rec[i].item1,rec[i].item2, rec[i].supCode, rec[i].donor, rec[i].shipnum, rec[i].quan);
	}

	fclose(fp);
}

void listDistribution()
{
	struct tempRecord inventoryRecord[1];
	fp = fopen("dist.txt", "r");
	if (fp == NULL)
	{
		printf("File does not exist.");

	}
	else
	{
		rewind(fp);
		printf("List of Distribution: \n");
		while (!feof(fp))
		{
			int ret = fscanf(fp, "%s%f", &inventoryRecord[0].supCode, &inventoryRecord[0].quan);
			if (ret == -1) break;
			else {
				printf("%s\t", inventoryRecord[0].supCode);
				printf("%.2f\n", inventoryRecord[0].quan);
			}
	
		}
	}
	fclose(fp);
	
}


		



