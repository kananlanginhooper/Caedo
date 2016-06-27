<?php

// don't really have to instantiate, no constructor
// $SiteConfigs = new clsSiteConfigs();

/////////////////////////////// $Url, $Site, $Env, $IsLocal, $WriteDatabase, $HasReadReplica, $ReadDatabase, $AllowURLOverride, $URLOverridePassKey, $SessionManager, $SessionManagerPath, $TidyUp, $ShowErrors, $ShowStats
$SiteConfig = new clsSiteConfig('localhost', 'default', '', true, 'caedo', false, '', false, '', 'file', '/tmp', true, true, true);
clsSiteConfigs::AddSiteConfig($SiteConfig);

// clone for IP access
$SiteConfig = clone $SiteConfig;
$SiteConfig->Url = '127.0.0.1';
clsSiteConfigs::AddSiteConfig($SiteConfig);

// clone for Example site
$SiteConfig = clone $SiteConfig;
$SiteConfig->Url = 'getcaedo.com';

// disable these for the live site
$SiteConfig->ShowErrors = false;
$SiteConfig->ShowStats = false;
clsSiteConfigs::AddSiteConfig($SiteConfig);

// Second example site
$SiteConfig = clone $SiteConfig;
$SiteConfig->Url = 'nukq.com';
clsSiteConfigs::AddSiteConfig($SiteConfig);

//
// Config will attempt to match the requested URL to the URL listed in the config
// if there is a match it will take it, otherwise if $DefaultSite is set, it will use that site
clsSiteConfigs::$DefaultSite = 'localhost';
