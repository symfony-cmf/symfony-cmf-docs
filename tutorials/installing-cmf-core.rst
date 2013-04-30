Installing and configuring the CMF core
=======================================

The goal of this tutorial is to install the minimal CMF components ("core")
with the minimum necessary configuration. From there, you can begin incorporating
CMF functionality into your application as needed.

This is aimed at experienced user who want to know all about the Symfony CMF
details. If this is your first encounter with the Symfony CMF it would be a
good idea to start with:

- :doc:`../getting-started/installing-symfony-cmf` page for instructions on how to quickly install the CMF (recommended for development)
- :doc:`../cookbook/installing-cmf-sandbox` for instructions on how to install a demonstration sandbox.

.. index:: RoutingBundle, CoreBundle, MenuBundle, ContentBundle, SonataBlockBundle, KnpMenuBundle, install

Preconditions
-------------
- `Installation of Symfony2 <http://symfony.com/doc/2.1/book/installation.html>`_ (2.1.x)
- :doc:`installing-configuring-doctrine-phpcr-odm`

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file

.. code-block:: javascript

    "minimum-stability": "dev",
    "require": {
        ...
        "symfony-cmf/symfony-cmf": "1.0.*"
    }

And then run

.. code-block:: bash

    php composer.phar update


Initialize bundles
~~~~~~~~~~~~~~~~~~

Next, initialize the bundles in ``AppKernel.php`` by adding them to the ``registerBundles`` method

.. code-block:: php

    // app/AppKernel.php

    public function registerBundles()
    {
        $bundles = array(
            // ...

            new Symfony\Cmf\Bundle\RoutingBundle\SymfonyCmfRoutingBundle(),
            new Symfony\Cmf\Bundle\CoreBundle\SymfonyCmfCoreBundle(),
            new Symfony\Cmf\Bundle\MenuBundle\SymfonyCmfMenuBundle(),
            new Symfony\Cmf\Bundle\ContentBundle\SymfonyCmfContentBundle(),
            new Symfony\Cmf\Bundle\BlockBundle\SymfonyCmfBlockBundle(),

            // Dependencies of the SymfonyCmfMenuBundle
            new Knp\Bundle\MenuBundle\KnpMenuBundle(),

            // Dependencies of the SymfonyCmfBlockBundle
            new Sonata\BlockBundle\SonataBlockBundle(),
        );

        // ...
    }

.. note::

    This also enables the PHPCR-ODM and related dependencies; setup instructions
    can be found in the dedicated documentation.


Configuration
-------------

To get your application running, very little configuration is needed.

Minimum configuration
~~~~~~~~~~~~~~~~~~~~~

These steps are needed to ensure your AppKernel still runs.

If you haven't done so already, make sure you have followed these steps from
:doc:`installing-configuring-doctrine-phpcr-odm`:

- Initialize ``DoctrinePHPCRBundle`` in ``app/AppKernel.php``
- Ensure there is a ``doctrine_phpcr:`` section in ``app/config/config.yml``
- Add the ``AnnotationRegistry::registerFile`` line to ``app/autoload.php``

Configure the BlockBundle in your ``config.yml``:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        sonata_block:
            default_contexts: [cms]

Additional configuration
~~~~~~~~~~~~~~~~~~~~~~~~

Because most CMF components use the DynamicRouter from the RoutingBundle, which by default is
not loaded, you will need to enable it as follows:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_routing:
            chain:
                routers_by_id:
                    symfony_cmf_routing.dynamic_router: 200
                    router.default: 100
            dynamic:
                enabled: true

You might want to configure more on the dynamic router, i.e. to automatically choose controllers based on content.
See :doc:`../bundles/routing` for details.

For now this is the only configuration we need. Mastering the configuration of the different
bundles will be handled in further tutorials. If you're looking for the configuration of a
specific bundle take a look at the corresponding :doc:`bundles entry<../index>`.
