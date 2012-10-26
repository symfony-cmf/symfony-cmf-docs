Installing and configuring CMF
==============================
The goal of this tutorial is to get you up and running with an application build on top of
(some of the bundles of) the Symfony Content Management Framework.

If this is your first encounter with the Symfony CMF it would be a good idea to first take a
look at `the big picture <http://slides.liip.ch/static/2012-01-17_symfony_cmf_big_picture.html#1>`_
and/or the `CMF sandbox environment <https://github.com/symfony-cmf/cmf-sandbox>`_ which is a
pre-installed Symfony / CMF application containing all CMF components. It is also available online
at `cmf.liip.ch<http://cmf.liip.ch>`.

If you want to create content in scripts, have a look at the
`fixtures loading code <https://github.com/symfony-cmf/cmf-sandbox/blob/master/src/Sandbox/MainBundle/DataFixtures/PHPCR/>`_..

If you just want to get started with a minimal installation it is recommended to use the
`Symfony2 CMF standard edition <https://github.com/symfony-cmf/symfony-cmf-standard>`_..

.. index:: RoutingExtraBundle, CoreBundle, MenuBundle, ContentBundle, SonataBlockBundle, KnpMenuBundle

Preconditions
-------------
- `Installation of Symfony2 <http://symfony.com/doc/master/index.html>`_
- :doc:`installing-configuring-doctrine-phpcr-odm`

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file

.. code-block:: javascript

    "require": {
        ...
        "symfony-cmf/symfony-cmf": "1.0.*"
    }

And then run

.. code-block:: bash

    php composer.phar update


Initialize bundles
~~~~~~~~~~~~~~~~~~

Next, initialize the bundles in ``app/AppKernel.php`` by adding them to the ``registerBundle`` method

.. code-block:: php

    public function registerBundles()
    {
        $bundles = array(
            // ...

            new Symfony\Cmf\Bundle\RoutingExtraBundle\SymfonyCmfRoutingExtraBundle(),
            new Symfony\Cmf\Bundle\CoreBundle\SymfonyCmfCoreBundle(),
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

To enable the dynamic router and to add the router to the routing chain add the
following to your project configuration

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing_extra:
            chain:
                routers_by_id:
                    symfony_cmf_routing_extra.dynamic_router: 20
                    router.default: 100
            dynamic:
                enabled: true

You might want to configure more on the dynamic router, i.e. to automatically choose controllers based on content.
See :doc:`../bundles/routing-extra`

For a basic functionality for the BlockBundle (required)

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_block:
            default_contexts: [cms]

For now this is the only configuration we need. Mastering the configuration of the different
bundles will be handled in further tutorials. If you're looking for the configuration of a
specific bundle take a look at the corresponding :doc:`bundles entry<../index>`.
