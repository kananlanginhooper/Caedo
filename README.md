# Caedo
Caedo PHP Framework.

IN PROGRESS.

I am working on spliting out all of the framework code I can to make it compatible with composer.  I feel being composer compatible is important, it will further reduse the number of files developers working projects.

I will list another project as an example that has the Caedo Framework as a composer dependency.  This project will show how to setup the config files, databases, etc.

I will also include links to zip archives that can be downloaded for quick on the go setup, novice developers and developers without composer support (eg some shared hosting).


TODO: 

Remove unneeded directories and sample pages.

changed /__framework to /__CAEDO

__vendors to vendor
__StaticIncludes to __STATICINCLUDES

changed the names of some files, like the config.*.inc files, also moved them to /__CAEDO

also changed caedo.php to __CAEDO.php

We want to make anything that's framework start with __ and be caps at the root level.  It'll make it easier to know what can and cannot be deleted.



__ is for framework files and classes, _ is for local (user) files and classes used by the framework



# Why?

Many people out there may be asking "why?". Why would you create a new
framework when there are already so many good options out there?

The main reason is that I didn't find a framework that fit my needs.  There are a lot of good frameworks out there, I've done my research, but I didn't find one that filled my need for a blend of simplicity and complexity.


# Basics
There are three main classes involved in rendering pages.
	
	__CAEDO/BasePages/BasePage.class.inc 
	This class deals with the mechanics of loading pages and controling MVC. This class is part of the framework and shoud be looked at an non-editable.  It is very generic, and should allow you to do anything you need by expanding it's defaults rather than changing the file.
	
	_local/PageTemplates/DefaultPageTemplate.php
	This is where we can start to code, the class name can be whatever we want. We inherit from this class for our pages. Its used to put the header and footer on each page so they don't have to be coded into each page class. This is also a great place to put any CSS that will be widely used and also common javascript. If the project uses jQuery, it should be loaded here, as it will be used for most pages. 
	
	This class it totally optional, you can have you page classes inherit from the base class, but using this class will save time and allow for smaller page class files
	
	Page Class file.
	This is the file that contains the page content. It is also in the folder that shows as URL, meaning there is no fancy re-writing or apache magic happening to make the framework work. There is a '/pages' folder, it contains a 'basics' folder and there is a file in the folder called index.php.
	
Each class inherits from the class above it, and can add information.  It is best to keep the most informaion in the Page Template to make the Page class file as small and simple as posible.  This will make it easy to create new pages without having to think to much about the template and layout.


# Javascript defaults

Caedo can be used with javascript framework.  I made the decision support jQuery and bootstrap our of the box, so if you'd like to use them, you don't have to do anything.  If you'd like to not use them, then change /__CAEDO/config/framework.defaults.php, and set $UsejQuery = false; and $UseBootstrap = false;



