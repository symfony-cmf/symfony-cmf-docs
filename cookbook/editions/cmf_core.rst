.. index::
    single: Installing CMF Core

Installing and Configuring the CMF Core Bundles
===============================================

The goal of this tutorial is to install the minimal CMF components ("core")
with the minimum necessary configuration. From there, you can begin
incorporating CMF functionality into your application as needed.

This tutorial is aimed at the experienced user who wants to learn more
about the details of the Symfony CMF. If this is your first encounter with
the Symfony CMF it would be a good idea to start with :doc:`cmf_sandbox` for
instructions on how to install a demonstration sandbox.

.. index:: RoutingBundle, CoreBundle, MenuBundle, ContentBundle, SonataBlockBundle, KnpMenuBundle, install

Preconditions
-------------

* `Installation of Symfony`_
* :doc:`../../bundles/phpcr_odm/introduction`

Installation
------------

Download the Bundles
~~~~~~~~~~~~~~~~~~~~

Add the following to your ``composer.json`` file:

.. code-block:: javascript

    "require": {
        ...
        "symfony-cmf/symfony-cmf": "1.2.*"
    }

And then run:

.. code-block:: bash

    $ composer update

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
            new Sonata\CoreBundle\SonataCoreBundle(),
            new Sonata\BlockBundle\SonataBlockBundle(),
        );

        // ...
    }

Configuration
-------------

To get your application running, very little configuration is needed. If
you want to render menus from Twig, you need to enable this feature in the
KnpMenuBundle:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        knp_menu:
            twig: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://knplabs.com/schema/dic/menu" twig="true"/>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('knp_menu', array(
            'twig' => true,
        ));

.. caution::

    While the CMF bundles work out of the box without configuration, you
    will need to configure a storage layer. To use the default provided
    model classes, you need PHPCR-ODM as well. Setup instructions are in
    :doc:`../../bundles/phpcr_odm/introduction`.

When using PHPCR-ODM, enable support globally for all CMF bundles with:

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        cmf_core:
            persistence:
                phpcr:
                    enabled: true

    .. code-block:: xml

        <!-- app/config/config.xml -->
        <?xml version="1.0" encoding="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/core">
                <persistence>
                    <phpcr enabled="true"/>
                </persistence>
            </config>
        </container>

    .. code-block:: php

        // app/config/config.php
        $container->loadFromExtension('cmf_core', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled' => true,
                ),
            ),
        ));

Next Steps
----------

If you want to support multiple languages, have a look at
:doc:`../../bundles/phpcr_odm/multilang`.

Then have a look at the individual :doc:`bundles <../../bundles/index>` you are
interested in.

.. _`Installation of Symfony`: https://symfony.com/doc/current/setup.html
