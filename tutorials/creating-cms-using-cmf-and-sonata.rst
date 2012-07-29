Creating a CMS using CMF and Sonata
===================================
The goal of this tutorial is to create a simple content management system using the CMF and the Sonata admin bundle.

Documentation TODO
------------------
- expand intro and add steps to take
- describe prerequisites
- describe steps to make a CMS

Preconditions
-------------
- Installation of Symfony CMF

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file::

    "require": {
        ...
        "sonata-project/doctrine-phpcr-admin-bundle": "1.0.*",
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
            new Symfony\Cmf\Bundle\TreeBundle\SymfonyCmfTreeBundle(),
            new Sonata\jQueryBundle\SonatajQueryBundle(),
            new Sonata\AdminBundle\SonataAdminBundle(),
            new Sonata\DoctrinePHPCRAdminBundle\SonataDoctrinePHPCRAdminBundle(),
        );
        // ...
    }
    
Configuration
-------------
    
SonataAdminBundle
~~~~~~~~~~~~~~~~~
Add route in ``app/config/routing.yml`` ::

    admin:
        resource: '@SonataAdminBundle/Resources/config/routing/sonata_admin.xml'
        prefix: /admin

TODO: link to reference
