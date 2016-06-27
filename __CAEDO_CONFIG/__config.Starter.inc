<?php

// This file is required, but the the content can totally be changed. We just need to have site and db config setup, but you can employ aditional logic if needed
// Just as an FYI, this file is called after the class auto loading code.  So you can use classes, just call them by name and they will be autoloaded
// You CANNOT on the other hand use the database....because this is the database setup file.  I've made this mistake before, you have to bootstrap from somewhere.  I suggest using $_SERVER and using the URL based loading that's built in.
// Security security.  Security requirements are a lot more varried than some people choose to admit.  High security isn't always better security.  Sometimes there's a site where it's just better for all the stakeholders that it get up quick, and security isn't important at all.  Maybe it's shared hosting.  Maybe you gave the hosting and database credentials to a guy you met on the subway this morning because he sounded like he was interested in your cause and could help.  Or you could be NASA launching a rocket to the moon, and there are people everywhere trying to take down your site for attention.  Making everything as secure as possible is like having a vault door for your refrigerator.  If you don't need the security it provides, it's more of a pain in the ass than a feature. Security varries.</rant>

// So, depending on what security is decided on, you will need to select apropreate options.
// I have built in some basics.  That is that there are files that contain plain text usernames and passwords for our databases.
// If you are interested in setting up your servers ahead of time, you can add /__CAEDO_CONFIG/config.DB.inc to .gitignore and put it on the servers manually.  That file is read (by default) and used to setup the primary database connection.  The credentials are then removed from memory (not wiped securily, but removed so they can't be accessed by name on a later page).  This is usually the way I will go when working on a mid-range security project and there are out-sourced developers.  Production database credentials are need to know, in all but the smallest projects.
// For extra small projects, which I define as any project that doesn't involve money or contact the outside world (eg email) -- I will usually only have one database (production) and all devs will have full access.
// For small projects, these are extra small projects that have some transactional changes, such as sending emails, or changing some database record based on user input -- In these cases I will usually only have localhost and production databases.  All devs will usually have production access.
// For medium projects and up I will usually restrict production database access from everyone aside from the architect and DBAs.
// For large projects, I doubt you're using this framework.  If you are, please read this: this software is provided "as-is," without any express or implied warranty. In no event shall Kanan Langin Hooper be held liable for any damages arising from the use of the software.


// Session, timezone and other php_ini and general setup commands
include_once $RootFolder . '__CAEDO_CONFIG/SessionAndIni.inc';

// This file is used to setup your environment configuration
require_once $RootFolder . '__CAEDO_CONFIG/config.Site.inc';

// This file is used to setup your database connections
require_once $RootFolder . '__CAEDO_CONFIG/config.DB.inc';

// This file is used to setup your Memcache
// require_once $RootFolder . '__CAEDO_CONFIG/Memcache.inc';

// This file is used to setup your APC application cache
// require_once $RootFolder . '__CAEDO_CONFIG/APC.inc';

// This file is used to setup your Data Sets
// require_once $RootFolder . '__CAEDO_CONFIG/DatasetsDef.inc';

// This file is used to setup your Amazon Web Services (S3, CloudFront, EC2)
// require_once $RootFolder . '__CAEDO_CONFIG/AWS.inc';

// This file is used to setup your CDN
// require_once $RootFolder . '__CAEDO_CONFIG/CDN.inc';

// This file is used to setup your WebSockets
// require_once $RootFolder . '__CAEDO_CONFIG/WebSockets.inc';

// This file is used to setup your session configuration
// require_once $RootFolder . '__CAEDO_CONFIG/config.Session.inc';

// this file is where you can put local functions. If you want to use a different file, or put this file somewhere else, or delete it all together, go for it.
include_once $RootFolder . '__CAEDO_CONFIG/LocalFunctions.inc';

// When we get around to adding modules, this is where the code to load them should go


