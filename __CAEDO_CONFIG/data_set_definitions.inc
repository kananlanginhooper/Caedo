<?php

if($IsLocal) {
	$Timeout = $CrazyShort_Timeout = 36000; // 10 hours
}
else {
	$Timeout = 1800;
	$CrazyShort_Timeout = 30;
}

$DataSetDefinitions["ref_team_member"] = new clsDataSetDefinition("select * from ref_team_member where tmAccountActive = 1 and tmEmployeeID >= %%StartingKey%% and tmEmployeeID <= %%EndingKey%% order by tmEmployeeID", "tmEmployeeID", 0, 0, 10, 50, 0, 99999999, 1, $Timeout, 0, 0, 0, 0);

// ------------------------------------------------- clsDataSetDefinition($BaseQuery, $IndexedColumn, $idUnique, $IsText, $ApproxRows, $DefaultPageSize, $DefaultStartKey, $DefaultEndKey, $Cachable, $CacheExpires, $RunOffEndProtection, $NotFoundExpected, $GroupByVerify, $SortVerify)
$DataSetDefinitions["sys_DataSetTesting_char"] = new clsDataSetDefinition("select * from sys_datasettesting_char where dstKey >= '%%StartingKey%%' and dstKey <= '%%EndingKey%%' order by dstKey", "dstKey", 0, 1, 99999, 10, "", "ZZZZZZZZZZZ", 1, $Timeout, 1, 0, 1, 1);
$DataSetDefinitions["sys_DataSetTesting_int"] = new clsDataSetDefinition("select * from sys_datasettesting_int where dstKey >= %%StartingKey%% and dstKey <= %%EndingKey%% order by dstKey", "dstKey", 0, 0, 99999, 10, 0, 99999999, 1, $Timeout, 1, 0, 1, 1);
// $DataSetDefinitions["sys_test_int"] = new clsDataSetDefinition("select * from sys_test_int where dstKey >= %%StartingKey%% and dstKey <= %%EndingKey%% order by dstKey", "dstKey", 0, 0, 99999, 10, 0, 99999999, 1, $Timeout, 1, 0, 1, 1);


clsDataSetDefinitions::AddDataSetDefinitions($DataSetDefinitions);