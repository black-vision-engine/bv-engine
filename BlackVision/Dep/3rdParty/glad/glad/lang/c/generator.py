import os
import sys

from glad.lang.common.generator import Generator
from glad.lang.common.util import makefiledir


KHRPLATFORM = 'https://www.khronos.org/registry/egl/api/KHR/khrplatform.h'


class CGenerator(Generator):
    def open(self):
        suffix = ''
        if not self.spec.NAME == 'gl':
            suffix = '_{}'.format(self.spec.NAME)

        self.h_include = '<glad/glad{}.h>'.format(suffix)
        self._f_c = open(make_path(self.path, 'src',
                                   'glad{}.c'.format(suffix)), 'w')
        self._f_h = open(make_path(self.path, 'include',
                                   'glad', 'glad{}.h'.format(suffix)), 'w')

        khr_url = KHRPLATFORM
        if os.path.exists('khrplatform.h'):
            khr_url = 'file://' + os.path.abspath('khrplatform.h')

        khr = os.path.join(self.path, 'include', 'KHR')
        khrplatform = os.path.join(khr, 'khrplatform.h')
        if not os.path.exists(khrplatform):
            if not os.path.exists(khr):
                os.makedirs(khr)
            self.opener.urlretrieve(khr_url, khrplatform)

        return self

    def close(self):
        self._f_c.close()
        self._f_h.close()

    def generate_loader(self, features, extensions):
        f = self._f_c

        if self.spec.NAME in ('egl', 'wgl'):
            features = {'egl': [], 'wgl': []}

        written = set()
        for api, version in self.api.items():
            for feature in features[api]:
                f.write('static void load_{}(GLADloadproc load) {{\n'
                        .format(feature.name))
                if self.spec.NAME in ('gl', 'glx', 'wgl'):
                    f.write('\tif(!GLAD_{}) return;\n'.format(feature.name))
                for func in feature.functions:
                    f.write('\tglad_{0} = (PFN{1}PROC)load("{0}");\n'
                            .format(func.proto.name, func.proto.name.upper()))
                f.write('}\n')

            for ext in extensions[api]:
                if len(list(ext.functions)) == 0 or ext.name in written:
                    continue

                f.write('static void load_{}(GLADloadproc load) {{\n'
                        .format(ext.name))
                if self.spec.NAME in ('gl', 'glx', 'wgl'):
                    f.write('\tif(!GLAD_{}) return;\n'.format(ext.name))
                if ext.name == 'GLX_SGIX_video_source': f.write('#ifdef _VL_H_\n')
                if ext.name == 'GLX_SGIX_dmbuffer': f.write('#ifdef _DM_BUFFER_H_\n')
                for func in ext.functions:
                    # even if they were in written we need to load it
                    f.write('\tglad_{0} = (PFN{1}PROC)load("{0}");\n'
                            .format(func.proto.name, func.proto.name.upper()))
                if ext.name in ('GLX_SGIX_video_source', 'GLX_SGIX_dmbuffer'):
                    f.write('#else\n')
                    f.write('\t(void)load;\n')
                    f.write('#endif\n')
                f.write('}\n')

                written.add(ext.name)

            f.write('static void find_extensions{}(void) {{\n'.format(api.upper()))
            if self.spec.NAME in ('gl', 'glx', 'wgl'):
                f.write('\tget_exts();\n')
                for ext in extensions[api]:
                    f.write('\tGLAD_{0} = has_ext("{0}");\n'.format(ext.name))
            f.write('}\n\n')

            if api == 'glx':
                f.write('static void find_core{}(Display *dpy, int screen) {{\n'.format(api.upper()))
            elif api == 'wgl':
                f.write('static void find_core{}(HDC hdc) {{\n'.format(api.upper()))
            else:
                f.write('static void find_core{}(void) {{\n'.format(api.upper()))

            self.loader.write_find_core(f)
            if self.spec.NAME in ('gl', 'glx', 'wgl'):
                for feature in features[api]:
                    f.write('\tGLAD_{} = (major == {num[0]} && minor >= {num[1]}) ||'
                            ' major > {num[0]};\n'.format(feature.name, num=feature.number))
            if self.spec.NAME == 'gl':
                f.write('\tif (GLVersion.major > {0} || (GLVersion.major >= {0} && GLVersion.minor >= {1})) {{\n'.format(version[0], version[1]))
                f.write('\t\tmax_loaded_major = {0};\n'.format(version[0]))
                f.write('\t\tmax_loaded_minor = {0};\n'.format(version[1]))
                f.write('\t}\n')
            f.write('}\n\n')

            if api == 'glx':
                f.write('int gladLoad{}Loader(GLADloadproc load, Display *dpy, int screen) {{\n'.format(api.upper()))
            elif api == 'wgl':
                f.write('int gladLoad{}Loader(GLADloadproc load, HDC hdc) {{\n'.format(api.upper()))
            else:
                f.write('int gladLoad{}Loader(GLADloadproc load) {{\n'.format(api.upper()))

            self.loader.write_begin_load(f)

            if api == 'glx':
                f.write('\tfind_core{}(dpy, screen);\n'.format(api.upper()))
            elif api == 'wgl':
                f.write('\tfind_core{}(hdc);\n'.format(api.upper()))
            else:
                f.write('\tfind_core{}();\n'.format(api.upper()))

            for feature in features[api]:
                f.write('\tload_{}(load);\n'.format(feature.name))
            f.write('\n\tfind_extensions{}();\n'.format(api.upper()))
            for ext in extensions[api]:
                if len(list(ext.functions)) == 0:
                    continue
                f.write('\tload_{}(load);\n'.format(ext.name))

            self.loader.write_end_load(f)
            f.write('}\n\n')

        self.loader.write_header_end(self._f_h)

    def generate_types(self, types):
        f = self._f_h

        self.loader.write_header(f)
        self.write_api_header(f)

        for type in types:
            if not self.spec.NAME in ('egl',) and 'khronos' in type.raw:
                continue
            f.write((type.raw + '\n').lstrip().replace('        ', ' '))

    def generate_features(self, features):
        f = self._f_h
        write = set()
        if self.spec.NAME in ('wgl',):
            # These are already defined in windows.h
            pass
        elif self.spec.NAME in ('egl',):
            self.write_enums(f, set(), features)

            for feature in features:
                for func in feature.functions:
                    self.write_function_def(f, func)
        else:
            self.write_functions(f, write, set(), features)

        f = self._f_c
        self.write_code_head(f)
        self.loader.write(f)
        self.loader.write_has_ext(f)

        if self.spec.NAME in ('gl', 'glx', 'wgl'):
            for feature in features:
                f.write('int GLAD_{};\n'.format(feature.name))

        for func in write:
            self.write_function(f, func)

    def generate_extensions(self, extensions, enums, functions):
        write = set()
        written = set(enum.name for enum in enums) | \
                  set(function.proto.name for function in functions)

        f = self._f_h
        self.write_functions(f, write, written, extensions)

        f = self._f_c
        if self.spec.NAME in ('gl', 'glx', 'wgl'):
            for ext in set(ext.name for ext in extensions):
                f.write('int GLAD_{};\n'.format(ext))

        written = set()
        for ext in extensions:
            if ext.name == 'GLX_SGIX_video_source': f.write('#ifdef _VL_H_\n')
            if ext.name == 'GLX_SGIX_dmbuffer': f.write('#ifdef _DM_BUFFER_H_\n')
            for func in ext.functions:
                if func in write and func not in written:
                    self.write_function(f, func)
                    written.add(func)
            if ext.name in ('GLX_SGIX_video_source', 'GLX_SGIX_dmbuffer'): f.write('#endif\n')

    def write_functions(self, f, write, written, extensions):
        self.write_enums(f, written, extensions)

        for ext in extensions:
            f.write('#ifndef {0}\n#define {0} 1\n'.format(ext.name))
            if self.spec.NAME in ('gl', 'glx', 'wgl'):
                f.write('GLAPI int GLAD_{};\n'.format(ext.name))
            if ext.name == 'GLX_SGIX_video_source': f.write('#ifdef _VL_H_\n')
            if ext.name == 'GLX_SGIX_dmbuffer': f.write('#ifdef _DM_BUFFER_H_\n')
            for func in ext.functions:
                if not func.proto.name in written:
                    self.write_function_prototype(f, func)
                    write.add(func)
                written.add(func.proto.name)
            if ext.name in ('GLX_SGIX_video_source', 'GLX_SGIX_dmbuffer'): f.write('#endif\n')
            f.write('#endif\n')

    def write_enums(self, f, written, extensions):
        for ext in extensions:
            for enum in ext.enums:
                if not enum.name in written:
                    f.write('#define {} {}\n'.format(enum.name, enum.value))
                written.add(enum.name)

    def write_api_header(self, f):
        for api in self.api:
            if api == 'glx':
                f.write('GLAPI int gladLoad{}Loader(GLADloadproc, Display *dpy, int screen);\n\n'.format(api.upper()))
            elif api == 'wgl':
                f.write('GLAPI int gladLoad{}Loader(GLADloadproc, HDC hdc);\n\n'.format(api.upper()))
            else:
                f.write('GLAPI int gladLoad{}Loader(GLADloadproc);\n\n'.format(api.upper()))

    def write_code_head(self, f):
        f.write('#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include {}\n'.format(self.h_include))

    def write_extern(self, fobj):
        fobj.write('#ifdef __cplusplus\nextern "C" {\n#endif\n')

    def write_extern_end(self, fobj):
        fobj.write('#ifdef __cplusplus\n}\n#endif\n')

    def write_function_def(self, fobj, func):
        # write a function definition instead of a prototype.
        # e.g. egl uses that, since the main functions get linked in and not loaded through a function.
        fobj.write('{} {}('.format(func.proto.ret.to_c(), func.proto.name))
        fobj.write(', '.join('{} {}'.format(param.type.to_c(), param.name) for param in func.params))
        fobj.write(');\n')

    def write_function_prototype(self, fobj, func):
        fobj.write('typedef {} (APIENTRYP PFN{}PROC)({});\n'.format(
            func.proto.ret.to_c(), func.proto.name.upper(),
            ', '.join('{} {}'.format(param.type.to_c(), param.name) for param in func.params))
        )
        fobj.write('GLAPI PFN{}PROC glad_{};\n'.format(func.proto.name.upper(),
                                                       func.proto.name))
        fobj.write('#define {0} glad_{0}\n'.format(func.proto.name))

    def write_function(self, fobj, func):
        fobj.write('PFN{}PROC glad_{};\n'.format(func.proto.name.upper(),
                                                 func.proto.name))


def make_path(path, *args):
    path = os.path.join(path, *args)
    makefiledir(path)
    return path
