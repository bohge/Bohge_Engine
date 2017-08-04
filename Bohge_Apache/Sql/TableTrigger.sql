use litehub_info;
DELIMITER $$
CREATE TRIGGER url_trigger 
BEFORE INSERT ON liteinfo_cn
FOR EACH ROW
BEGIN
DECLARE next_id INT;
SET next_id = ( SELECT MAX(liteid) FROM liteinfo_cn ) + 1;
IF isnull( next_id ) then
SET next_id = 1;
end if;
SET NEW.urlbase = CONCAT( NEW.urlbase, next_id, "/" );
END$$
DELIMITER ;