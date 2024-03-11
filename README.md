Ansys Fluent does not have the IAPWS water tables - those are available in Ansys CFX.
CFX is a good solver, but many analysts prefer Fluent, e. g. because of its ability to use the polyhedral or cutcell mesh.

This project is aimed at providing IAPWS support into Ansys Fluent through a UDF.
The UDF loads a CSV table, which is stored into the RAM in Fluent as a 2D Array.

To obtain material properties, the UDF then calls bilinear interpolation of the tabled properties during the C_loop macro.

How does this program work?

1. use the Python iapws library wrapper to extract the material properties table (it extracts the csv)
2. the table is loaded as a CSV into the C program and converted into a 2D array of doubles (I am using a struct wrapper for convenience)
3. an algorithm is used to obtain two 1D arrays (struct wrappers) containing only the unique values of the independent variables (temperature in K, pressure in MPa)
4. during a C_loop macro, the cell temperature (C_T) and cell pressure (C_P) is obtained
5. the cell pressure and cell temperature and the unique value arrays (i1, i2) are fed into the findClosest2D function, which in turn returns the index of the closest value in the original 2D array
   - the searching function 2D uses searching function 1D.
       - the searching function 1D creates an array of absolute values of differences
       - the closest value is the index of the minimum value of the new temporary array
   - the searching function 1D is used on the first independent variable -> index1
   - the searching function 1D is used on the second independent variable -> index2
   - result: index1 * len(i2) + index2
6. the obtained index is fed into the getRectangle function, which creates an array of 4 points (rows of the table) around the value to be interpolated
7. the rectangle values and the searched values are fed into the bilinear interpolation, which is then fed into the material properties in Fluent.
