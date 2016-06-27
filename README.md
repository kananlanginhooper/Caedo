# Caedo Framework - Composer Core

This is the core of the Caedo PHP Framework.  This project is here for forking and patching.  This project updates the Composer/Packagist package 'repository caedo/framework'.  If you are looking to use the framework and not modify it, please go here: https://github.com/kananlanginhooper/Caedo_Example


The Caedo PHP Framework Example Project is located here: https://github.com/kananlanginhooper/Caedo_Example


# Framework Notes

Because this is the core project only, I'm not going to describe features here.  For that, please go to the example project (https://github.com/kananlanginhooper/Caedo_Example).  What we will cover here are upcoming changes, design desisions and reasoning for the choices that were made.  I hope this is a help in guiding the ongoing development effort.

#Class Naming Conventions and Caching

As I'm sure you are aware, caedo has a few requirements around classes.

- Classes must all start with cls and end with .class.inc.

so the class FileMonkey must be name clsFileMonkey.class.inc.  And then used as clsFileMonkey in the php code.
This may seem limiting to some people, but think it makes it extremly clear what names are classes.  Also having the .inc ending prevents the class from being executed directly, and lessens the need for security around protecting these files.  I've seen many (large) projects that expose internal files to be run by leaving them with a php extension.  The inc extension is a "secure by default" at least in apache php.

Input on the security of class files is something I'd be interested in hearing opinions on.

- Directory agnostic

We allow the class files to be put in any directory under the __LOCAL_USER_CLASSES directory.  This allows developers to setup their own methods of organizing their code.  They can put all classes in the root of __LOCAL_USER_CLASSES, or make the directory structure 10 folders deep, caedo doesn't care.

One draw back of this is a can make the class files hard for developers to find.  I view this is the developers own problem, you put the file there, it's your job to remember where you put it.

- Static Includes

This is something that I'm not sure I've seen a framework do.  Include by default the classes that were needed by this file last time.  My reasoning for this feature is spl_autoload is slow.  At the very least there is a error handeler for the missing class, then a directory/file search.  Because of the flexiblity awarded by being "Directory agnostic", there can be many places that class file could be.  The larger the project grows, the more time will be spent looking for these files, and also generally the more classes there will be used on a given page.  It also pains me to see the same ineffecent code being called on every page load.

Static Includes get around this by keeping the list of all files included with the spl_autoload, and loading that class code in by default.  Since this is a cache of include files, it can be deleted at any time.  It is also listed in .gitignore.  It should also be deleted anytime you are moving between two computers and the root of your webserver changes.

More code overhead than needed?  Yes, in some cases classes will be loaded that aren't needed for that page to execute.  I haven't seen this be a problem in the past.  If it is a problem for you and you have a use case, please send it to me, I'd be interested to see what you have.


# IN PROGRESS.

I am working on spliting out all of the framework code I can to make it compatible with composer.  I feel being composer compatible is important, it will further reduse the number of files developers working projects.

I will list another project as an example that has the Caedo Framework as a composer dependency.  This project will show how to setup the config files, databases, etc.

I will also include links to zip archives that can be downloaded for quick on the go setup, novice developers and developers without composer support (eg some shared hosting).


# TODO: 

Remove unneeded directories and sample pages.

changed /__framework to /__CAEDO

__vendors to vendor
__StaticIncludes to __STATICINCLUDES

changed the names of some files, like the config.*.inc files, also moved them to /__CAEDO

also changed caedo.php to __CAEDO.php

We want to make anything that's framework start with __ and be caps at the root level.  It'll make it easier to know what can and cannot be deleted.



__ is for framework files and classes, _ is for local (user) files and classes used by the framework



