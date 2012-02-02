Installing and configuring CMF
==============================
The goal of this tutorial is to get you up and running with an application build on top of (some of the bundles of) the Symfony Content Management Framework.

This does include the following bundles:
- SymfonyCmfCoreBundle
- SymfonyCmfContentBundle
- SymfonyCmfChainRoutingBundle
- SymfonyCmfMenuBundle

This tutorial does not include the installation and configuration of the following bundles:
- SymfonyCmfMultilangContentBundle
- SymfonyCmfPHPCRBrowserBundle
- SymfonyCmfTreeBundle

If this is your first encounter with the Symfony CMF it would be a good idea to first take a look at `the big picture <http://slides.liip.ch/static/2012-01-17_symfony_cmf_big_picture.html#1>`_ and/or the `CMF sandbox environment <https://github.com/symfony-cmf/symfony-cmf>`_ which is a pre-installed Symfony / CMF application containing all CMF components.
    
Prerequisites
-------------
- Installation of Doctrine PHPCR ODM

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Download the CMF bundles (and depencies) to the vendor folder. Add the following to your ``deps`` file::

    [symfony-cmf]
        git=http://github.com/symfony-cmf/symfony-cmf.git
        git_command=submodule update --init --recursive

    [ChainRoutingBundle]
        git=git://github.com/symfony-cmf/ChainRoutingBundle.git
        target=/bundles/Symfony/Cmf/Bundle/ChainRoutingBundle

    ;== Dependencies of the SymfonyCmfMenuBundle
    [knp-menu]
        git=https://github.com/KnpLabs/KnpMenu.git

    [KnpMenuBundle]
        git=http://github.com/KnpLabs/KnpMenuBundle.git
        target=/bundles/Knp/Bundle/MenuBundle

And run the vendors script to download the bundles::

    php bin/vendors install

After every vendor install/update make sure to run the following command in the symfony-cmf folder ``vendor/symfony-cmf/``::

    git submodule update --recursive --init


Register namespaces
~~~~~~~~~~~~~~~~~~~
Next step is to add the autoloader entries in ``app/autoload.php``::

    $loader->registerNamespaces(array(
        // ...

        'Symfony\\Cmf'     => array(__DIR__.'/../vendor/symfony-cmf/src', __DIR__.'/../vendor/bundles'),

        // Dependencies of the SymfonyCmfMenuBundle
        'Knp\\Menu'        => __DIR__.'/../vendor/knp-menu/src',
        'Knp\\Bundle'      => __DIR__.'/../vendor/bundles',
        
        // ...
    ));                                              .

Register annotations
~~~~~~~~~~~~~~~~~~~~
Add autoloader entries in ``app/autoload.php`` for the ODM annotations right after the last ``AnnotationRegistry::registerFile`` line::

    // ...
    AnnotationRegistry::registerFile(__DIR__.'/../vendor/symfony-cmf/vendor/doctrine-phpcr-odm/lib/Doctrine/ODM/PHPCR/Mapping/Annotations/DoctrineAnnotations.php');
    // ...

Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method::

    public function registerBundles()
    {
        $bundles = array(
            // ...

            new Symfony\Cmf\Bundle\ChainRoutingBundle\SymfonyCmfChainRoutingBundle(),
            new Symfony\Cmf\Bundle\CoreBundle\SymfonyCmfCoreBundle(),
            new Symfony\Cmf\Bundle\ContentBundle\SymfonyCmfContentBundle(),
            new Symfony\Cmf\Bundle\MenuBundle\SymfonyCmfMenuBundle(),
            
            // Dependencies of the SymfonyCmfMenuBundle
            new Knp\Bundle\MenuBundle\KnpMenuBundle(),

        );
        // ...
    }

Configuration
-------------
To get your application running very little configuration is needed. But because the SymfonyCmfMenuBundle is dependent of the doctrine router you need to explicitly enable the doctrine router as per default it is not loaded.

To enable the doctrine router and to add the router to the routing chain add the following to ``app/config/config.yml``::

    symfony_cmf_chain_routing:
        chain:
            routers_by_id:
                symfony_cmf_chain_routing.doctrine_router: 200
                router.default: 100
        doctrine:
            enabled: true

For now this is the only configuration we need. Mastering the configuration of the different bundles will be handled in further tutorials. If you're looking for the configuration of a specific bundle take a look at the reference (TODO link).