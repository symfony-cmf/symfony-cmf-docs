.. index::
    single: Search; Bundles

SearchBundle
============

    The SearchBundle provides integration with `LiipSearchBundle`_ to provide a
    site wide search.

Installation
------------

You can install this bundle `with composer`_ using the
`symfony-cmf/search-bundle`_ package.

Routing
~~~~~~~

Finally, add the relevant routing to your configuration

.. configuration-block::

    .. code-block:: yaml

        cmf_search:
            resource: "@CmfSearchBundle/Resources/config/routing/phpcr/search.xml"

    .. code-block:: xml

        <?xml version="1.0" encoding="UTF-8" ?>
        <routes xmlns="http://symfony.com/schema/routing"
            xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
            xsi:schemaLocation="http://symfony.com/schema/routing
                http://symfony.com/schema/routing/routing-1.0.xsd">

            <import resource="@CmfSearchBundle/Resources/config/routing/phpcr/search.xml" />
        </routes>

    .. code-block:: php

        use Symfony\Component\Routing\RouteCollection;

        $collection = new RouteCollection();
        $fileName = "@CmfSearchBundle/Resources/config/routing/phpcr/search.xml";
        $collection->addCollection($loader->import($fileName));

        return $collection;

Read On
-------

* :doc:`configuration`

.. _`LiipSearchBundle`: https://github.com/liip/LiipSearchBundle
.. _`symfony-cmf/search-bundle`: https://packagist.org/packages/symfony-cmf/search-bundle
.. _`with composer`: http://getcomposer.org

