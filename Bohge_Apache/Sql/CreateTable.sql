use litehub_info;

CREATE TABLE liteinfo_cn
(
	liteid			int 			NOT NULL 	AUTO_INCREMENT		PRIMARY	KEY	,
	litename		nchar (10)		NOT NULL,
	requestversion	int		 		NOT NULL,
    urlbase			text 			NOT NULL
);