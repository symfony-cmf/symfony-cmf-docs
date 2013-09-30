MediaBundle Configuration
=========================

The MediaBundle provides a way to store and edit any media and provides a
generic base of common interfaces and models that allow the user to build media
management solutions for a CMS. It can be configured under the ``cmf_media``
key in your application configuration. When using XML, you can use the
``http://cmf.symfony.com/schema/dic/media`` namespace.

Configuration
-------------

.. _config-media-persistence:

persistence
~~~~~~~~~~~

phpcr
.....

This defines the persistence driver. The default configuration of persistence
is the following configuration:

.. configuration-block::

    .. code-block:: yaml

        cmf_media:
            persistence:
                phpcr:
                    enabled: false
                    manager_name: ~
                    media_basepath: /cms/media
                    media_class: Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Media
                    file_class: Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\File
                    directory_class: Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Directory
                    image_class: Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Image

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">

            <config xmlns="http://cmf.symfony.com/schema/dic/media">
                <persistence>
                    <phpcr
                        enabled="false"
                        manager-name="null"
                        media-basepath="/cms/media"
                        media-class="Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Media"
                        file-class="Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\File"
                        directory-class="Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Directory"
                        image-class="Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Image"
                    />
                </persistence>
            </config>

        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_media', array(
            'persistence' => array(
                'phpcr' => array(
                    'enabled' => false,
                    'manager_name' => null,
                    'media_basepath' => '/cms/media',
                    'media_class' => 'Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Media',
                    'file_class' => 'Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\File',
                    'directory_class' => 'Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Directory',
                    'image_class' => 'Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Image',
                ),
            ),
        ));


enabled
"""""""

**type**: ``boolean`` **default**: ``false``

If ``true``, PHPCR is enabled in the service container.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.enabled``.

PHPCR can be enabled by multiple ways such as:

.. configuration-block::

    .. code-block:: yaml

        phpcr: ~ # use default configuration
        # or
        phpcr: true # straight way
        # or
        phpcr:
            manager_name: ... # or any other option under 'phpcr'

    .. code-block:: xml

        <persistence>
            <!-- use default configuration -->
            <phpcr />

            <!-- or setting it the straight way -->
            <phpcr>true</phpcr>

            <!-- or setting an option under 'phpcr' -->
            <phpcr manager-name="..." />
        </persistence>

    .. code-block:: php

        $container->loadFromExtension('cmf_block', array(
            // ...
            'persistence' => array(
                'phpcr' => null, // use default configuration
                // or
                'phpcr' => true, // straight way
                // or
                'phpcr' => array(
                    'manager_name' => '...', // or any other option under 'phpcr'
                ),
            ),
        ));

media_basepath
""""""""""""""

**type**: ``string`` **default**: ``/cms/media``

The basepath for CMS media documents in the PHPCR tree.

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``%cmf_core.persistence.phpcr.basepath%/media``.

manager_name
""""""""""""

**type**: ``string`` **default**: ``null``

The name of the Doctrine Manager to use. ``null`` tells the manager registry to
retrieve the default manager.<persistence>

If the :doc:`CoreBundle <../../bundles/core/index>` is registered, this will default to
the value of ``cmf_core.persistence.phpcr.manager_name``.

media_class
"""""""""""

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Media'``

The class for media objects. Be they cloud hosted or local files.

file_class
""""""""""

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\File'``

The class for objects representing a file.

directory_class
"""""""""""""""

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Directory'``

The class for objects representing directories.

image_class
"""""""""""

**type**: ``string`` **default**: ``'Symfony\Cmf\Bundle\MediaBundle\Doctrine\Phpcr\Image'``

The class for image objects. This just adds methods to get the native image
dimensions, but implicitly also tells applications that this object is suitable
to view with an <img> HTML tag.

upload_file_role
~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``ROLE_CAN_UPLOAD_FILE``

The role used to protect the default upload action of the file and image
controller of the MediaBundle.

upload_file_helper_service_id
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``null``

The service id to override the default service the alias
``cmf_media.upload_file_helper`` points to.

upload_image_helper_service_id
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

**type**: ``string`` **default**: ``null``

The service id to override the default service the alias
``cmf_media.upload_image_helper`` points to.

use_jms_serializer
~~~~~~~~~~~~~~~~~~

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the serializer handler for images is activated. If set to
``auto``, this is activated only if the JMSSerializerBundle is present.

The serializer handler adds an url to the serialized representation of an image
object.

use_elfinder
~~~~~~~~~~~~

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, the elfinder adaper is activated and ``cmf_media.default_browser``
is set. If set to ``auto``, this is activated only if the FMElfinderBundle is
present.

use_imagine
~~~~~~~~~~~

**type**: ``enum`` **valid values**: ``true|false|auto`` **default**: ``auto``

If ``true``, imagine related parameters are set. If set to ``auto``, this is
activated only if the LiipImagineBundle is present.

If enabled and if phpcr is enabled, the imagine data loader and cache
invalidation listener are activated.

The LiipImagineBundle is able to provide scaled images. Otherwise images are
always provided in the original resolution (and scaling might happen in the
browser through the img width and height attributes).

imagine_filters
~~~~~~~~~~~~~~~

.. configuration-block::

    .. code-block:: yaml

        cmf_media:
            imagine_filters:
                upload_thumbnail:   image_upload_thumbnail
                elfinder_thumbnail: elfinder_thumbnail

    .. code-block:: xml

        <config xmlns="http://cmf.symfony.com/schema/dic/media">
            <imagine-filter
                upload_thumbnail="image_upload_thumbnail"
                elfinder_thumbnail="elfinder_thumbnail"
            />
        </config>

    .. code-block:: php

        $container->loadFromExtension('cmf_media', array(
            'imagine_filters'     => array(
                'upload_thumbnail'   => 'image_upload_thumbnail',
                'elfinder_thumbnail' => 'elfinder_thumbnail',
            ),
        ));

upload_thumbnail
................

**type**: ``string`` **default**: ``image_upload_thumbnail``

The imagine filter to be used as thumbnail for uploaded images.

elfinder_thumbnail
..................

**type**: ``string`` **default**: ``elfinder_thumbnail``

The imagine filter to be used for elfinder thumbnails.

extra_filters
~~~~~~~~~~~~~

**prototype**: ``array``

This is a list of filters that should be invalidated when images are uploaded.
(a LiipImagineBundle shortcoming that we can't just invalidate a file in all
caches in one go)

.. configuration-block::

    .. code-block:: yaml

        cmf_media:
            extra_filters:
                - imagine_filter_name1
                - imagine_filter_name2

    .. code-block:: xml

        <config xmlns="http://cmf.symfony.com/schema/dic/media">
            <extra-filter>imagine_filter_name1</extra-filter>
            <extra-filter>imagine_filter_name2</extra-filter>
        </config>

    .. code-block:: php

        $container->loadFromExtension('cmf_media', array(
            'extra_filters'      => array(
                'imagine_filter_name1',
                'imagine_filter_name2',
            ),
        ));
