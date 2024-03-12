#include "TOPR_13_ARRAY_INTERPOLATION.h"
Array1D createUniqueValuesArray1D(Array1D arrayIpt)
{
	fprintfWrapper_newline_head("createUniqueValuesArray1D: start!");

	int position_1 = 0;
	int position_2 = 1;
	int uniqueVals = countUniqueVals1D(arrayIpt);

	fprintfWrapper_newline_mid_upper("createUniqueValuesArray1D: reached unique value -> Breaking at %d unique values!", uniqueVals);
	

	Array1D nuArray = createArrayStruct1D(uniqueVals);
	fprintfWrapper_newline_mid_upper("createUniqueValuesArray1D: creating unique 1D array!");

	nuArray.array[position_1] = arrayIpt.array[position_1];
	for (position_1 = 0; position_1 < arrayIpt.count; position_1++)
	{
		if (!valueInArray(arrayIpt.array[position_1], nuArray))
		{
			nuArray.array[position_2] = arrayIpt.array[position_1];
			fprintfWrapper_newline_mid_lower("nuArray[%d]: %lf \t arrayIpt[%d]: %lf",
				position_2,
				nuArray.array[position_2],
				position_1,
				arrayIpt.array[position_1]
			);
			position_2++;
		}
	}
	printArrayStruct1D(nuArray);
	fprintfWrapper_newline_tail("createUniqueValuesArray1D: success! -> %d values", nuArray.count);
	return nuArray;
}

Array1D extractColumnFromTable(Array2D arrayIpt, int selectedColumn)
{
	fprintfWrapper_newline_head("extractColumnFromTable: start!");
	int rowIndex_arrayIpt;
	fprintfWrapper_newline_mid_upper("extractColumnFromTable: getting the numLines!");

	fprintfWrapper_newline_mid_upper("extractColumnFromTable: extracted column will have %d lines!", arrayIpt.rows);
	Array1D nuArray = createArrayStruct1D(arrayIpt.rows);

	fprintfWrapper_newline_mid_upper("extractColumnFromTable: initialized the column!");
	fprintfWrapper_newline_mid_upper("extractColumnFromTable: filling the column!");
	for (rowIndex_arrayIpt = 0; rowIndex_arrayIpt < arrayIpt.rows; rowIndex_arrayIpt++)
	{
		nuArray.array[rowIndex_arrayIpt] = arrayIpt.array[rowIndex_arrayIpt][selectedColumn];
		fprintfWrapper_newline_mid_lower("nuArray[%d]: %lf \t arrayIpt[%d][%d]: %lf",
			rowIndex_arrayIpt,
			nuArray.array[rowIndex_arrayIpt],
			rowIndex_arrayIpt,
			selectedColumn,
			arrayIpt.array[rowIndex_arrayIpt][selectedColumn]
		);
	}
	printArrayStruct1D(nuArray);
	fprintfWrapper_newline_tail("extractColumnFromTable: success!");
	return nuArray;
}

Array1D differences1D(double value, Array1D arrayIpt)
{
	fprintfWrapper_newline_head("differences1D: start -> value: %lf", value);
	Array1D nuArray = createArrayStruct1D(arrayIpt.count);
	int index;

	fprintfWrapper_newline_mid_upper("differences1D: filling the array with abs. value of differences!");
	for (index = 0; index < nuArray.count; index++)
	{
		nuArray.array[index] = absDiff(arrayIpt.array[index], value);
		fprintfWrapper_newline_mid_lower("nuArray.array[%d] = %lf",index, nuArray.array[index]);
	}
	fprintfWrapper_newline_head("differences1D: success!");
	return nuArray;
}

Array1D createRandom1D(double loBound, double hiBound, int Count)
{
	fprintfWrapper_newline_head("createRandom1D: start -> <%lf, %lf>_%d", loBound, hiBound, Count);
    Array1D nuArray;
    nuArray.array   = ardrand(loBound, hiBound, Count);
    nuArray.count   = Count;
    nuArray.last    = Count-1;
	fprintfWrapper_newline_head("createRandom1D: success!");
    return nuArray;
}

Array1D lerp1D(Array1D inputVals, Array1D independentVals, Array1D dependentVals)
{
	fprintfWrapper_newline_head("lerp1D: start -> %d values!", min(min(independentVals.count, dependentVals.count), inputVals.count));
    if (independentVals.count != dependentVals.count || independentVals.count != inputVals.count || dependentVals.count != inputVals.count)
    {
        fprintfWrapper_newline_tail("lerp1D: failure -> the arrays have different sizes [%d,  %d, %d]", inputVals.count, independentVals.count, dependentVals.count);
        exit(1);
    }
    Array1D nuArray = createArrayStruct1D(independentVals.count);
    fprintfWrapper_newline_mid_upper("lerp1D: initialized the array!");
    fprintfWrapper_newline_mid_upper("lerp1D: lerping!");
    int index = 0;
    for (index = 0; index < nuArray.count; index++)
    {
        nuArray.array[index] = interpolation1D_linear(independentVals, dependentVals, inputVals.array[index]);
        fprintfWrapper_newline_mid_lower("index: %d [%lf <-> %lf, %lf] = %lf", index, inputVals.array[index], independentVals.array[index], dependentVals.array[index], nuArray.array[index]);
    }
	fprintfWrapper_newline_head("lerp1D: success!");
    return nuArray;
}

Array2D getRectangle(int index, Array2D arrayIpt, int yUnique)
{
	fprintfWrapper_newline_head("getRectangle: start -> target index: %d [%lf, %lf], array length: %d, array offset: %d",
	index, arrayIpt.array[index][T], arrayIpt.array[index][P],
	arrayIpt.count, yUnique
	);

	fprintfWrapper_newline_mid_upper("getRectangle: index MOD yUnique -> %d MOD yUnique %d = %d", index, yUnique, index%yUnique);

	if (index < 0 || index >= arrayIpt.lastRow)
	{
		fprintfWrapper_newline_tail("getRectangle: failure -> target index: %d < 0 or %d > %d", index, index, arrayIpt.lastRow);
		exit(1);
	}




	int TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT;
	Array2D nuArray = createArrayStruct2D(4, arrayIpt.columns);
	fprintfWrapper_newline_mid_upper("getRectangle: rectangle initialized!");

	if (index % yUnique == 0) //special case: first pressure value
	{
		fprintfWrapper_newline_mid_upper("getRectangle: index MOD yUnique == 0 -> %d MOD %d -> index+= (yUnique+1)", index, yUnique);
		index = index + yUnique + 1;
		
	}

	else if (index % yUnique == yUnique-1) // special case: last pressure value
	{	
		if (index != arrayIpt.lastRow) // only last temperature
			{
			fprintfWrapper_newline_mid_upper("getRectangle: index MOD yUnique == 0 -> %d MOD %d -> index+= (yUnique-1)", index, yUnique);
			index = index + yUnique - 1;
		}

		else // special case: both last temperature and last pressure
		{
			fprintfWrapper_newline_mid_upper("getRectangle: index MOD yUnique == 0 -> %d MOD %d -> index+= -1", index, yUnique);
			index = index - 1;
	
		}
		
	}



	// left edge value
	if ((index < yUnique))
	{
		fprintfWrapper_newline_mid_upper("getRectangle: v1 -> x in (0, yUnique)\t %d in (0, %d)", index, yUnique);
		TOP_LEFT	= index;
		TOP_RIGHT	= TOP_LEFT	+ 1;
		BOT_LEFT	= TOP_LEFT	+ yUnique;
		BOT_RIGHT	= TOP_RIGHT	+ yUnique;
	}

	// right edge value
	else if (	(index >= (arrayIpt.lastRow - yUnique))	)
	{	

		// have not surpassed the bounds yet
		if (index <= arrayIpt.lastRow)
		{
			fprintfWrapper_newline_mid_upper("getRectangle: v2A -> x in (lastRow - yUnique, lastRow)\t %d in (%d, %d)",
			index, (arrayIpt.lastRow-yUnique), arrayIpt.lastRow
			);
			TOP_LEFT	= index		- yUnique;
			TOP_RIGHT	= TOP_LEFT	+ 1;
			BOT_LEFT	= TOP_LEFT	+ yUnique;
			BOT_RIGHT	= TOP_RIGHT	+ yUnique;
		}
		
		// the bounds have been surpassed
		else
		{
			fprintfWrapper_newline_mid_upper("getRectangle: v2B -> x > lastRow\t %d > %d",
			index, arrayIpt.lastRow
			);
			TOP_LEFT	= index		- 2*yUnique;
			TOP_RIGHT	= TOP_LEFT	+ 1;
			BOT_LEFT	= index 	- 1*yUnique;
			BOT_RIGHT	= TOP_RIGHT	+ yUnique;
		}
	}

	//middle value
	else
	{		
		fprintfWrapper_newline_mid_upper("getRectangle: v3 -> x in (yUnique, lastRow - yUnique)\t %d in (%d, %d)",
		index, yUnique, (arrayIpt.lastRow-yUnique)
		);
		TOP_LEFT	= index - yUnique - 1;
		TOP_RIGHT	= TOP_LEFT	+ 2;
		BOT_LEFT	= TOP_LEFT	+ yUnique;
		BOT_RIGHT	= TOP_RIGHT	+ yUnique;
	}
	
	nuArray.array[TL] = arrayIpt.array[TOP_LEFT];
	nuArray.array[BL] = arrayIpt.array[BOT_LEFT];
	nuArray.array[TR] = arrayIpt.array[TOP_RIGHT];
	nuArray.array[BR] = arrayIpt.array[BOT_RIGHT];
	printArrayStruct2D(nuArray);
	fprintfWrapper_newline_tail("getRectangle: success! -> [[%d, %d], [%d, %d]]", TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT);
	return nuArray;
}

Array2D readFromCSV(char *filePath)
{
	fprintfWrapper_newline_head("readFromCSV: start -> from %s!", filePath);
	double stringToNumber;
	int position	= 0;
	int lineNum	= 0; 
	int rowCount 	= countLinesFile_noHeader(filePath);
	int columnCount = countColumnsFile(filePath);

	fprintfWrapper_newline_mid_upper("readFromCSV: creating Array2D with %d rows and %d columns!", rowCount, columnCount);
	Array2D nuArray = createArrayStruct2D(rowCount, columnCount);
	char **headers = (char**)malloc(sizeof(char*)*columnCount); 
	char line[BUFFER_SIZE_STRING];
	char *data;

	fprintfWrapper_newline_mid_upper("readFromCSV: filling Array2D with empty arrays 1D");
	for (position = 0; position < rowCount; position++)
	{
		nuArray.array[position] = (double*)malloc(sizeof(double)*columnCount);
	}
	fprintfWrapper_newline_mid_upper("readFromCSV: filled Array2D with empty arrays 1D");
	FILE* file = fopen(filePath, "r");
	if (file != NULL)
	{
		fprintfWrapper_newline_mid_upper("readFromCSV: reading %s", filePath);
		while (fgets(line, sizeof(line), file))
		{

			fprintfWrapper_newline_mid_lower("line: %s", line);
			if (line[0] == '\n') {continue;}
			int columnIndex	= 0;
			data = strtok(line, ", ");


			while (data != NULL)
			{
				////free(data);
				//fprintfWrapper_newline_mid_upper("readFromCSV: data = %s", data);
				if (lineNum == 0)
				{
					//fprintfWrapper_newline_mid_upper("readFromCSV: debugLine1 -> data: %s", data);
					headers[columnIndex] = strdup(data); /* aby mi zustala data, porad odkazuji pointery, tak musim string v headeru duplikovat */
					//fprintfWrapper_newline_mid_upper("readFromCSV: debugLine2");					
					fprintfWrapper_comma("%s", headers[columnIndex]);
				}

				else if (lineNum > 0)
				{
					//fprintfWrapper_newline_mid_upper("readFromCSV: got through header!");
					sscanf(data, "%lf", &stringToNumber); /* na adresu d prevedu hodnotu ze stringu na double */
					if (lineNum % ITER_PRINT_FREQUENCY == 0)
					{
						fprintfWrapper_comma("%.6lf", stringToNumber);
					}
					nuArray.array[lineNum - 1][columnIndex] = stringToNumber;
				}
				//fprintfWrapper_newline_mid_upper("readFromCSV: inner while loop before strtok");
				data = strtok(NULL, ", ");
				//fprintfWrapper_newline_mid_upper("readFromCSV: next inner while loop iter!");
				columnIndex++;
			}

			if (lineNum % ITER_PRINT_FREQUENCY == 0) {newline(1);}
			if (lineNum > rowCount*columnCount)
			{
				fprintfWrapper_newline_mid_upper("readFromCSV: Exceeded the length of a file, how?");
				break;
			}
			//fprintfWrapper_newline_mid_upper("readFromCSV: next outer while loop iter!");
			lineNum++;
		}
		//if (lineNum < 50) {newline(1);}
		if (lineNum == ITER_PRINT_FREQUENCY) {fprintfWrapper_newline_mid_upper("readFromCSV: End of table preview!");}
		nuArray.headers = headers;

		fprintfWrapper_newline_mid_upper("readFromCSV: closing %s", filePath);
		fclose(file);

		printArrayStruct2D(nuArray);
		fprintfWrapper_newline_tail("readFromCSV: success -> read %s into a 2D array with %d rows and %d columns", filePath, nuArray.rows, nuArray.columns);
		return nuArray;
	}
	fprintfWrapper_newline_tail("readFromCSV: failure -> does not exist: %s", filePath);
	exit(1);
}