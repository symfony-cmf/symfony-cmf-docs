Installing and configuring simple CMS
=====================================

The goal of this tutorial is to install and configure simple CMS.
Note that Symfony2.1 is required for the CMF to work.

The SimpleCmsBundle provides a solution to easily map content, routes and menu
items based on a single tree structure in the content repository.

For more information for now see the documentation of the
`SymfonyCmfSimpleCmsBundle <https://github.com/symfony-cmf/SimpleCmsBundle#readme>`_

For a simple example installation of the bundle check out the
`Symfony CMF Standard Edition <https://github.com/symfony-cmf/symfony-cmf-standard>`_

The `CMF website <https://github.com/symfony-cmf/symfony-cmf-website/>`_ is
another application using the SimpleCmsBundle.


.. index:: SimpleCmsBundle

Preconditions
-------------
- :doc:`/tutorials/installing-configuring-cmf` (TODO: make this tutorial more self-contained)
- Or :doc:`/reference/routing-extra` needs to be installed manually, optionally also :doc:`/reference/menu` and :doc:`/reference/block`

Installation
------------

Download the bundles
~~~~~~~~~~~~~~~~~~~~
Add the following to your ``composer.json`` file

.. code-block:: javascript

    "require": {
        ...
        "symfony-cmf/simple-cms-bundle": "1.0.*"
    }

And then run

.. code-block:: bash

    php composer.phar update


Initialize bundles
~~~~~~~~~~~~~~~~~~
Next, initialize the bundle in ``app/AppKernel.php`` by adding it to the ``registerBundle`` method.

.. code-block:: php

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

.. configuration-block::

    .. code-block:: yaml

        # app/config/config.yml
        symfony_cmf_simple_cms:
            routing:
                templates_by_class:
                    Symfony\Cmf\Bundle\SimpleCmsBundle\Document\Page:  SymfonyCmfSimpleCmsBundle:Page:index.html.twig

For the full reference, see :doc:`/reference/simple-cms`.