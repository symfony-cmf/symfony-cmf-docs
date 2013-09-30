.. index::
    single: Search; Bundles

SearchBundle
============

    The SearchBundle provides integration with `LiipSearchBundle`_ to provide a
    site wide search.

Installation
------------

You can install the bundle in 2 different ways:

* Use the official Git repository (https://github.com/symfony-cmf/SearchBundle);
* Install it via Composer (``symfony-cmf/search-bundle`` on `Packagist`_).

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

.. _`LiipSearchBundle`: https://github.com/liip/LiipSearchBundle
.. _`Packagist`: https://packagist.org/packages/symfony-cmf/search-bundle
