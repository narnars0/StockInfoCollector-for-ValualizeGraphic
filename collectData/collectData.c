#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "collectData.h"
#include "../max.h"
#include "../path.h"

int main()
{
	FILE* stockListFile;
	FILE* dataFile;
	FILE* collectFile;

	char company[MAX_COMPANY];
	char url[MAX_URL];
	char symbol[MAX_SYMBOL];
	char scriptBlock[MAX_BLOCK];

	char year[5];
	char month[3];
	char day[3];
	char currentTime[11];

	char pathHtml[MAX_PATH];
	char pathDataFile[MAX_PATH];
	char pathCollectFile[MAX_PATH];

	char closedPrice[MAX_DATA];
	char vol[MAX_DATA];
	char marketCap[MAX_DATA];

	char equationVol[MAX_DATA];
	char equationMarketCap[MAX_DATA];

	char* pch1;
	char* pch2;

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	sprintf(year, "%d", tm.tm_year + 1900);
	sprintf(month, "%02d", tm.tm_mon + 1);
	sprintf(day, "%02d", tm.tm_mday);

	strcpy(currentTime, year);
	strcat(currentTime, "-");
	strcat(currentTime, month);
	strcat(currentTime, "-");
	strcat(currentTime, day);

	strcpy(pathCollectFile, "../data/");
	strcat(pathCollectFile, currentTime);
	strcat(pathCollectFile, ".data");
	
	stockListFile = fopen("../stock.list", "r");
	collectFile = fopen(pathCollectFile, "w");

	while(fscanf(stockListFile, "%s %s %s", company, symbol, url) != EOF)
	{
		memset(closedPrice, '\0', MAX_DATA);
		memset(vol, '\0', MAX_DATA);
		memset(marketCap, '\0', MAX_DATA);

		strcpy(pathHtml, "../data_html/");
		strcat(pathHtml, currentTime);
		strcat(pathHtml, "/");
		strcat(pathHtml, company);
		strcat(pathHtml, "/");
		
		strcpy(pathDataFile, pathHtml);
		strcat(pathDataFile, company);
		strcat(pathDataFile, ".data");

		dataFile = fopen(pathDataFile, "r");

		fscanf(dataFile, "%s", closedPrice);
		fscanf(dataFile, "%s", vol);
		fscanf(dataFile, "%s", marketCap);

		printf("DEBUG: closedPrice %s vol %s marketCap %s\n", closedPrice, vol, marketCap);
		
		memset(equationVol, '\0', MAX_DATA);
		memset(equationMarketCap, '\0', MAX_DATA);

		strcpy(equationVol, vol);
		strcpy(equationMarketCap, marketCap);

		pch1 = strchr(vol, 'M');
		
		if(pch1 != NULL)
		{
			strcpy(equationVol, "=");
			strncat(equationVol, vol, pch1 - vol);
			strcat(equationVol, "*10^6");
		}

		pch1 = strchr(vol, 'B');

		if(pch1 != NULL)
		{
			strcpy(equationVol, "=");
			strncat(equationVol, vol, pch1 - vol);
			strcat(equationVol, "*10^9");
		}

		pch1 = strchr(vol, 'T');

		if(pch1 != NULL)
		{
			strcpy(equationVol, "=");
			strncat(equationVol, vol, pch1 - vol);
			strcat(equationVol, "*10^12");
		}
		
		pch2 = strchr(marketCap, 'M');

		if(pch2 != NULL)
		{
			strcpy(equationMarketCap, "=");
			strncat(equationMarketCap, marketCap, pch2 - marketCap);
			strcat(equationMarketCap, "*10^6");
		}

		pch2 = strchr(marketCap, 'B');

		if(pch2 != NULL)
		{
			strcpy(equationMarketCap, "=");
			strncat(equationMarketCap, marketCap, pch2 - marketCap);
			strcat(equationMarketCap, "*10^9");
		}

		pch2 = strchr(marketCap, 'T');

		if(pch2 != NULL)
		{
			strcpy(equationMarketCap, "=");
			strncat(equationMarketCap, marketCap, pch2 - marketCap);
			strcat(equationMarketCap, "*10^12");
		}

		fprintf(collectFile, "%s\t%s\t%s\n", closedPrice, equationVol, equationMarketCap);
		
		fclose(dataFile);
	}
	
	fclose(stockListFile);
	fclose(collectFile);

	return 0;
}
