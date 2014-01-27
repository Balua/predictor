CREATE TABLE "info" (
    "id" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    "source" TEXT NOT NULL CHECK(source = "meteo_ist" or source = "noaa"),
    "pred_date" TEXT NOT NULL,
    "path" TEXT NOT NULL 
);






