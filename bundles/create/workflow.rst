Workflows
---------

CreateJS uses a REST api for creating, loading and changing content. To delete content
the HTTP method DELETE is used. Since deleting might be a more complex operation
than just removing the content form the storage (e.g. getting approval by another
editor) there is no simple delete button in the user frontend. Instead, CreateJS and
CreatePHP use ``workflows`` to implement that. This bundle comes with a simple implementation
of a workflow to delete content. To enable the workflow set the config option ``delete`` to true.

.. configuration-block::

    .. code-block:: yaml

        cmf_create:
            persistence:
                phpcr:
                    delete: true

    .. code-block:: xml

        <?xml version="1.0" charset="UTF-8" ?>
        <container xmlns="http://symfony.com/schema/dic/services">
            <config xmlns="http://cmf.symfony.com/schema/dic/create">
                <persistence>
                    <phpcr
                        delete="true"
                    >
                    </phpcr>
                </persistence>
            </config>
        </container>

    .. code-block:: php

        $container->loadFromExtension('cmf_create', array(
            'persistence' => array(
                'phpcr' => array(
                    'delete' => true,
                ),
            ),
        ));

This results in the delete workflow being registered with CreatePHP and CreateJS so that
you can now delete content from the frontend.
