using PCRE;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace MetaGenerator
{
    internal class MethodParser : MemberParser
    {
        public MethodParser(string text) : base(text)
        {
        }

        public MethodInfo[] Parse()
        {
            IEnumerable<PcreMatch> matches = FindMatches(@"METHOD(?<attr>\((?:[^()]|(?&attr))*\))\s*([\w\s:<>()*&,]+)\s+(\w+)\s*(?<args>\((?:[^()]|(?&args))*\))\s*([\w\s:<>()*&]+)?(=)?", Text);

            if (matches.Count() == 0)
                return null;

            return ParseInfos(matches);
        }

        private MethodInfo[] ParseInfos(IEnumerable<PcreMatch> matches)
        {
            var infos = MemberInfo.Construct<MethodInfo>(matches.Count());

            int i = 0;
            foreach (var match in matches)
            {
                infos[i].name = match.Groups[3].Value;
                infos[i].attributeInfos = ParseAttributes(match.Groups["attr"].Value);
                infos[i].virtualType = ParseVirtualType(match);
                infos[i].declarationInfo = ParseDeclarationInfo(match);

                ++i;
            }

            return infos;
        }

        private static MethodVirtualType ParseVirtualType(PcreMatch match)
        {
            if (match.Groups[6].Success)
                return MethodVirtualType.PureVirtual;

            if (match.Groups[2].Value.Contains("virtual") || match.Groups[5].Value.Contains("override"))
                return MethodVirtualType.Virtual;

            return MethodVirtualType.NotVirtual;
        }

        private static MethodInfo.DeclarationInfo ParseDeclarationInfo(PcreMatch match)
        {
            return new()
            {
                parameters = match.Groups["args"].Value.Trim(),
                prefix = match.Groups[2].Value.Trim(),
                suffix = Regex.Replace(match.Groups[5].Value.Trim(), @"\boverride\b", string.Empty)
            };
        }
    }
}
