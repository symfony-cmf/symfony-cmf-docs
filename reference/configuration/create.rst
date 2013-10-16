CreateBundle configuration
==========================

The CreateBundle provides frontend editing based on create.js and CreatePHP.
It can be configured under the ``cmf_create`` key in your application
configuration. When using XML you should use the
``http://cmf.symfony.com/schema/dic/create`` namespace.

.. note::

    The CreateBundle provides no model classes of its own. It still needs to
    know what persistence layer you are using, in order to decide what service
    to use to interact with the storage layer during save operations.

Configuration
-------------

Security
~~~~~~~~

The controller that receives save requests from create.js requires the user to
have a specific role to control who is allowed to edit content. As it would
not be convenient to show the create.js editor to users not allowed to edit the
site, the controller loading the create.js javascripts with the
``includeJSFilesAction`` also checks this role. If the image controller is
activated, it checks for this role as well.


.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            role: ROLE_ADMIN

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu"
                role="ROLE_ADMIN"
            />
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'role' => 'ROLE_ADMIN',
        ));

.. _config-create-persistence:

persistence
~~~~~~~~~~~

This defines the persistence driver and associated classes. The default
persistence configuration has the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_menu:
            object_mapper_service_id: ~
            persistence:
                phpcr:
                    enabled:              false
                    manager_name:         ~
                    image:
                        enabled:          false
                        model_class:      ~
                        controller_class: Symfony\Cmf\Bundle\CreateBundle\Controller\ImageController
                        basepath:         /cms/media

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu">
                <persistence>
                    <phpcr
                        enabled="false"
                        manager-name="null"
                    >
                        <image
                            enabled="false"
                            model-class="null"
                            controller-class="Symfony\Cmf\Bundle\CreateBundle\Controller\ImageController"
                            basepath="/cms/media"
                        />
                    </phpcr>
                </persistence>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_menu', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled' => false,
                    'manager_name' => null,
                    'image' => array(
                        'enabled' => false,
                        'model_class' => null,
                        'controller_class' => 'Symfony\Cmf\Bundle\CreateBundle\Controller\ImageController',
                        'basepath' => '/cms/media',
                    ),
                ),
            ),
        ));

object_mapper_service_id
""""""""""""""""""""""""

You can specify a service implementing ``Midgard\CreatePHP\RdfMapperInterface``
that will handle objects that need to be stored by the REST handler of
CreatePHP. You need to either specify this service or enable the phpcr
persistence for this bundle to work.

enabled
"""""""

.. include:: partials/persistence_phpcr_enabled.rst.inc

manager_name
""""""""""""

.. include:: partials/persistence_phpcr_manager_name.rst.inc

image
"""""

These settings are only used with the optional hallo editor. The default
CKEditor uses the :doc:`ELfinder plugin <../../bundles/media/adapters/elfinder>`
provided by the MediaBundle.

If you need different image handling, you can either overwrite
``model_class`` and/or the ``controller_class``.

Metadata Handling
~~~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            auto_mapping: true
            rdf_config_dirs:
                - "%kernel.root_dir%/Resources/rdf-mappings"
            map:
                '<http://rdfs.org/sioc/ns#Post>': 'Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent'

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu"
                auto-mapping="true"
            >
                <rdf-config-dir>%kernel.root_dir%/Resources/rdf-mappings</rdf-config-dir>
                <map name="'<http://rdfs.org/sioc/ns#Post>'">Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent</map>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'auto_mapping' => true,
            'rdf_config_dirs' => array('%kernel.root_dir%/Resources/rdf-mappings'),
            'map' => array('<http://rdfs.org/sioc/ns#Post>' => 'Symfony\Cmf\Bundle\ContentBundle\Doctrine\Phpcr\StaticContent'),
        ));

auto_mapping
""""""""""""

If not set to false, the CreateBundle will look for mapping files in every
bundle in the directory ``Resource/rdf-mappings``.

rdf_config_dirs
"""""""""""""""

Additional directories to look for mapping files besides the auto mapped
directory.

map
"""

Mapping from RDF type name to class. This configuration is used when adding
items to collections. *Note that collection support is currently incomplete
in the CreateBundle.*

REST handler
~~~~~~~~~~~~

You can configure the REST handler class with the ``rest_controller_class``
option.

Editor configuration
~~~~~~~~~~~~~~~~~~~~

You can tweak a few things on the editor. Most of the time, the only relevant
setting is the ``plain_text_types``.

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            plain_text_types: ['dcterms:title']
            editor_base_path: /bundles/cmfcreate/vendor/ckeditor/
            fixed_toolbar: true
            stanbol_url: http://dev.iks-project.eu:8081

    .. code-block:: xml

        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/menu"
                editor-base-path="/bundles/cmfcreate/vendor/ckeditor/"
                fixed-toolbar="true"
                stanbol_url="http://dev.iks-project.eu:8081"
            >
                <plain-text-type>dcterms:title</plain-text-type>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'plain_text_types' => array('dcterms:title'),
            'editor_base_path' => '/bundles/cmfcreate/vendor/ckeditor/',
            'fixed_toolbar' => true,
            'stanbol_url' => 'http://dev.iks-project.eu:8081',
        ));

plain_text_types
""""""""""""""""

A list of RDFa field types that will be edited with a plain text editor without
any formatting options. All other fields are edited with the WYSIWYG options
activated.

editor_base_path
""""""""""""""""

If you use a non-standard setup, you can adjust the editor base path
configuration. This is only relevant for CKEditor.

fixed_toolbar
"""""""""""""

Fix the editor toolbar on top of the page. Currently only supported by the
hallo.js editor.

stanbol_url
"""""""""""

Apache stanbol can be used for semantic enhancement of content. This feature
can optionally be used with the hallo.js editor.
