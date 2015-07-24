.. index::
    single: SimpleCms; Bundles
    single: SimpleCmsBundle

SimpleCmsBundle
===============

    The SimpleCmsBundle provides a simplistic CMS on top of the CMF components
    and bundles.

While the core CMF components focus on flexibility, the simple CMS trades away
some of that flexibility in favor of simplicity.

The SimpleCmsBundle provides a solution to easily map content, routes and menu
items based on a single tree structure in the content repository.

You can find an introduction to the bundle in the
:doc:`Getting started <../../book/simplecms>` section.

The `CMF website`_ is an application using the SimpleCmsBundle.

.. tip::

    For a simple example installation of the bundle check out the
    `Symfony CMF Standard Edition`_

Installation
------------

You can install this bundle `with composer`_ using the
``symfony-cmf/simple-cms-bundle`` package on `Packagist`_.

This bundle integrates the routing, content and menu bundles of the CMF. All of
them and their dependencies need to be instantiated in the kernel::

    // app/AppKernel.php

    // ...
    class AppKernel extends Kernel
    {
        public function registerBundles()
        {
            $bundles = array(
                // ...
                new Knp\Bundle\MenuBundle\KnpMenuBundle(),
                new Symfony\Cmf\Bundle\MenuBundle\CmfMenuBundle(),
                new Symfony\Cmf\Bundle\ContentBundle\CmfContentBundle(),
                new Symfony\Cmf\Bundle\RoutingBundle\CmfRoutingBundle(),
            );

            // ...
        }

        // ...
    }

Sections
--------

* :doc:`multilang`
* :doc:`rendering`
* :doc:`extending_page_class`

.. _`Symfony CMF Standard Edition`: https://github.com/symfony-cmf/standard-edition
.. _`CMF website`: https://github.com/symfony-cmf/cmf-website/
.. _`with composer`: http://getcomposer.org
.. _`Packagist`: https://packagist.org/packages/symfony-cmf/simple-cms-bundle
