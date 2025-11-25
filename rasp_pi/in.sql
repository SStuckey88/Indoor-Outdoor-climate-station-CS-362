.open climatedata.db

Create Table oClimate (
    time BIGINT UNSIGNED Primary Key,
    oTemp INT,
    hum FLOAT,
    light INT,
    press FLOAT,
);