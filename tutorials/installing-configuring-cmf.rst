Installing and configuring CMF
==============================
The goal of this tutorial is to get you up and running with an application build on top of
(some of the bundles of) the Symfony Content Management Framework.

If this is your first encounter with the Symfony CMF it would be a good idea to first take a
look at `the big picture <http://slides.liip.ch/static/2012-01-17_symfony_cmf_big_picture.html#1>`_
and/or the `CMF sandbox environment <https://github.com/symfony-cmf/symfony-cmf>`_ which is a
pre-installed Symfony / CMF application containing all CMF components.

Preconditions
-------------
- `Installation of Symfony2 <http://symfony.com/doc/master/index.html>`_
- :doc:`/tutorials/installing-configuring-doctrine-phpcr-odm`

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file::

    "require": {
        ...
        "symfony-cmf/symfony-cmf": "1.0.*"
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

            new Symfony\Cmf\Bundle\RoutingExtraBundle\SymfonyCmfRoutingExtraBundle(),
            new Symfony\Cmf\Bundle\CoreBundle\SymfonyCmfCoreBundle(),
            new Symfony\Cmf\Bundle\MultilangContentBundle\SymfonyCmfMultilangContentBundle(),
            new Symfony\Cmf\Bundle\MenuBundle\SymfonyCmfMenuBundle(),
            new Symfony\Cmf\Bundle\ContentBundle\SymfonyCmfContentBundle(),
            new Symfony\Cmf\Bundle\BlockBundle\SymfonyCmfBlockBundle(),

            // Dependencies of the SymfonyCmfMenuBundle
            new Knp\Bundle\MenuBundle\KnpMenuBundle(),

            // Dependencies of the SymfonyCmfBlockBundle
            new Sonata\CacheBundle\SonataCacheBundle(),
            new Sonata\BlockBundle\SonataBlockBundle(),

        );
        // ...
    }

Note that this also installs the PHPCR ODM and related dependencies, setup instructions
can be found in the dedicated documentation.

Configuration
-------------
To get your application running very little configuration is needed. But because the
SymfonyCmfMenuBundle is dependent of the doctrine router you need to explicitly enable
the doctrine router as per default it is not loaded.

To enable the dynamic router and to add the router to the routing chain add the following to ``app/config/config.yml``::

    symfony_cmf_routing_extra:
        chain:
            routers_by_id:
                symfony_cmf_routing_extra.dynamic_router: 20
                router.default: 100
        dynamic:
            enabled: true

For a basic functionality for the BlockBundle (required)::

    sonata_block:
        default_contexts: [cms]

If you are *NOT* using Sonata Admin Bundle the following configuration is needed::

    symfony_cmf_menu:
        use_sonata_admin: false

For now this is the only configuration we need. Mastering the configuration of the different
bundles will be handled in further tutorials. If you're looking for the configuration of a
specific bundle take a look at the reference (TODO link).
