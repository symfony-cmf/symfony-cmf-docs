SymfonyCmfCoreBundle
====================
This is the core bundle for the Symfony2 content management framework. See `the documentation<https://github.com/symfony-cmf/symfony-cmf-docs>`_ for more information about the CMF.

This bundle provides common functionality and utilities for the other cmf bundle.

Configuration
-------------
Configuration of this bundle

mainmenu_basepath
~~~~~~~~~~~~~~~~~
**type**: ``string``  **default**: ``/cms/navigation/main``

routing_basepath
~~~~~~~~~~~~~~~~~
**type**: ``string``  **default**: ``/cms/routes``

content_basepath
~~~~~~~~~~~~~~~~~
**type**: ``string``  **default**: ``/cms/content``

filestore_basepath
~~~~~~~~~~~~~~~~~
**type**: ``string``  **default**: ``%kernel.root_dir%/../../web/filestore``

filestore_relative_basepath
~~~~~~~~~~~~~~~~~
**type**: ``string``  **default**: ``filestore``