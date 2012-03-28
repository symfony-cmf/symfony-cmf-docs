SymfonyCmfContentBundle
=======================
This bundle provides a document for static content and the controller to render it.

Dependencies
------------
This bundle is shipped within the symfony-cmf git repository and is normally not used independently. 

Configuration
-------------
The configuration key for this bundle is ``symfony_cmf_content``

document_class
~~~~~~~~~~~~~~~~~
**type**: ``string``  **default**: ``Symfony\Cmf\Bundle\ContentBundle\Document\StaticContent``

default_template
~~~~~~~~~~~~~~~~~
**type**: ``string``  **default**: ``SymfonyCmfContentBundle:StaticContent:index.html.twig``

static_basepath
~~~~~~~~~~~~~~~~~
**type**: ``string``  **default**: ``/cms/content/static``