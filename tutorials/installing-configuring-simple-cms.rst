Installing and configuring simple CMS
=====================================
The goal of this tutorial is to install and configure simple CMS.
Note that Symfony2.1 (currently master) is required for the CMF to work.

The SimpleCmsBundle provides a solution to easily map content, routes and menu items
based on a single content structure in the content repository.

For more information for now see the documentation of the `SymfonyCmfSimpleCmsBundle <https://github.com/symfony-cmf/SimpleCmsBundle#readme>`_

For a simple example installation of check out the `Symfony CMF Standard Edition <https://github.com/symfony-cmf/symfony-cmf-standard>`_

Preconditions
-------------
- :doc:`/tutorials/installing-configuring-cmf`
- Or - :doc:`/reference/routing-extra`, :doc:`/reference/menu` and :doc:`/reference/block` need to be installed manually

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file::

    "require": {
        ...
        "symfony-cmf/simple-cms-bundle": "1.0.*"
    }

And then run::

    php composer.phar update

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method::

    public function registerBundles()
    {
        $bundles = array(
            // ...

            new Symfony\Cmf\Bundle\SimpleCmsBundle\SymfonyCmfSimpleCmsBundle(),

        );
        // ...
    }
    
Configuration
-------------
Next step is to configure the bundles.

Basic configuration, add to ``app/config/config.yml``::

    symfony_cmf_simple_cms:
        routing:
            templates_by_class:
                Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page:  SymfonyCmfSimpleCmsBundle:Page:index.html.twig
