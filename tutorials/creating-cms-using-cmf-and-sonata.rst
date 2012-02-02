Creating a CMS using CMF and Sonata
===================================
The goal of this tutorial is to create a simple content management system using the CMF and the Sonata admin bundle.

Documentation TODO
------------------
- expand intro and add steps to take
- describe prerequisites
- describe steps to make a CMS

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``deps`` file::

    [SonatajQueryBundle]
        git=http://github.com/sonata-project/SonatajQueryBundle.git
        target=/bundles/Sonata/jQueryBundle

    [SonataAdminBundle]
        git=https://github.com/sonata-project/SonataAdminBundle.git
        target=/bundles/Sonata/AdminBundle

    [SonataDoctrinePHPCRAdminBundle]
        git=https://github.com/sonata-project/SonataDoctrinePhpcrAdminBundle.git
        target=/bundles/Sonata/DoctrinePHPCRAdminBundle

    [TreeBundle]
        git=git://github.com/symfony-cmf/TreeBundle.git
        target=/bundles/Symfony/Cmf/Bundle/TreeBundle
        
And run the vendors script to download the bundles::

    php bin/vendors install

Register namespaces
~~~~~~~~~~~~~~~~~~~
Next step is to add the autoloader entries in ``app/autoload.php``::

    $loader->registerNamespaces(array(
        // ...
        'Sonata'           => __DIR__.'/../vendor/bundles',
        // ...
    ));

        
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