.. index::
    single: Installing CMF Core

Installing and Configuring the CMF Core
=======================================

.. include:: ../_outdate-caution.rst.inc

The goal of this tutorial is to install the minimal CMF components ("core")
with the minimum necessary configuration. From there, you can begin
incorporating CMF functionality into your application as needed.

This tutorial is aimed at the experienced user who wants to learn more
about the details of the Symfony CMF. If this is your first encounter with
the Symfony CMF it would be a good idea to start with:

* :doc:`../../book/installation` page for instructions on
  how to quickly install the CMF (recommended for development)
* :doc:`cmf_sandbox` for instructions on how to install
  a demonstration sandbox.

.. index:: RoutingBundle, CoreBundle, MenuBundle, ContentBundle, SonataBlockBundle, KnpMenuBundle, install

Preconditions
-------------

* `Installation of Symfony2`_ (2.1.x)
* :doc:`../installing_configuring_doctrine_phpcr_odm`

Installation
------------

Download the Bundles
~~~~~~~~~~~~~~~~~~~~

Add the following to your ``composer.json`` file:

.. code-block:: javascript

    "minimum-stability": "dev",
    "require": {
        ...
        "symfony-cmf/symfony-cmf": "1.0.*"
    }

And then run:

.. code-block:: bash

    $ php composer.phar update

Initialize bundles
~~~~~~~~~~~~~~~~~~

Next, initialize the bundles in ``AppKernel.php`` by adding them to the
``registerBundles`` method::

    // app/AppKernel.php

    // ...
    public function registerBundles()
    {
        $bundles = array(
            // ...

            new Symfony\Cmf\Bundle\RoutingBundle\CmfRoutingBundle(),
            new Symfony\Cmf\Bundle\CoreBundle\CmfCoreBundle(),
            new Symfony\Cmf\Bundle\MenuBundle\CmfMenuBundle(),
            new Symfony\Cmf\Bundle\ContentBundle\CmfContentBundle(),
            new Symfony\Cmf\Bundle\BlockBundle\CmfBlockBundle(),

            // Dependencies of the CmfMenuBundle
            new Knp\Bundle\MenuBundle\KnpMenuBundle(),

            // Dependencies of the CmfBlockBundle
            new Sonata\BlockBundle\SonataBlockBundle(),
        );

        // ...
    }

.. note::

    This also enables the PHPCR-ODM and related dependencies; setup
    instructions can be found in the dedicated documentation.

Configuration
-------------

To get your application running, very little configuration is needed.

Minimum Configuration
~~~~~~~~~~~~~~~~~~~~~

These steps are needed to ensure your ``AppKernel`` still runs.

If you haven't done so already, make sure you have followed these steps from
:doc:`../installing_configuring_doctrine_phpcr_odm`:

* Initialize ``DoctrinePHPCRBundle`` in ``app/AppKernel.php``
* Ensure there is a ``doctrine_phpcr:`` section in ``app/config/config.yml``
* Add the ``AnnotationRegistry::registerFile`` line to ``app/autoload.php``

Configure the BlockBundle in your ``config.yml``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_block:
            default_contexts: [cms]

Additional Configuration
~~~~~~~~~~~~~~~~~~~~~~~~

Because most CMF components use the DynamicRouter from the RoutingBundle,
which by default is not loaded, you will need to enable it as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_routing:
            chain:
                routers_by_id:
                    cmf_routing.dynamic_router: 200
                    router.default: 100
            dynamic:
                enabled: true

You might want to configure more on the dynamic router, i.e. to automatically
choose controllers based on content.  See
:doc:`../../bundles/routing/introduction` for details.

For now this is the only configuration we need. Mastering the configuration of
the different bundles will be handled in further articles. If you're looking
for the configuration of a specific bundle take a look at the corresponding
:doc:`bundles reference <../../reference/index>`.

.. _`Installation of Symfony2`: http://symfony.com/doc/2.1/book/installation.html
