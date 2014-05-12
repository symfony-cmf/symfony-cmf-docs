ContentBundle configuration
===========================

The ContentBundle provides a document and controller for static content and
can be configured under the ``cmf_content`` key in your application
configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/content`` namespace.

Configuration
-------------

default_template
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``null``

This defines the template to use when rendering the content if none is
specified in the route. ``{_format}`` and ``{_locale}`` are replaced with the
request format and the current locale.

.. _config-content-persistence:

persistence
~~~~~~~~~~~

This defines the persistence driver. The default configuration of persistence
is the following configuration:


.. configuration-block::

    .. code-block:: yaml

        cmf_content:
            persistence:
                phpcr:
                    enabled:          false
                    admin_class:      ~
                    document_class:   ~
                    content_basepath: /cms/content
                    use_sonata_admin: auto

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/content">
                <persistence>
                    <phpcr
                        enabled="false"
                        admin_class="null"
                        document-class="null"
                        content-basepath="/cms/content"
                        use-sonata-admin="auto"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_content', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled'          => false,
                    'admin_class'      => null,
                    'document_class'   => null,
                    'content_basepath' => '/cms/content',
                    'use_sonata_admin' => 'auto',
        ));

enabled
*******

.. include:: partials/persistence_phpcr_enabled.rst.inc

admin_class
***********

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\ContentBundle\Admin\StaticContentAdmin``

The admin class to use when :ref:`Sonata Admin is activated <quick-tour-third-party-sonata>`.

document_class
**************

**type**: ``string`` **default**: ``Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent``

The Content class to use.

content_basepath
****************

**type**: ``string`` **default**: ``/cms/content``

The basepath for Content documents in the PHPCR tree.

use_sonata_admin
****************

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the admin classes for SimpleCmsBundle pages are activated. If set
to ``auto``, the admin services are activated only if the
SonataPhpcrAdminBundle is present.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will
default to the value of ``cmf_core.persistence.phpcr.use_sonata_admin``.
