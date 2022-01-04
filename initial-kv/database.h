void setupDB();
void closeDB();
int putIntoDB(int key, char *value);
int getFromDB(int key);
int getAllFromDB();
int clearDB();
int deleteFromDB(int key);