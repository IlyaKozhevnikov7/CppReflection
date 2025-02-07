﻿using PCRE;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;

namespace MetaGenerator
{
    internal class FieldParser : SecondaryMemberParser
    {
        public FieldParser(string text) : base(text)
        {
        }

        public FieldInfo[] Parse()
        {
            IEnumerable<PcreMatch> matches = FindMatches(@"FIELD(?<attr>\((?:[^()]|(?&attr))*\))([\s\S]*?;)", Text);
            return ParseInfos(matches);
        }

        private FieldInfo[] ParseInfos(IEnumerable<PcreMatch> matches)
        {
            var infos = MemberInfoBase.Construct<FieldInfo>(matches.Count());

            int i = 0;
            foreach (var match in matches)
            {
                var fieldDeclaration = match.Groups[2].Value;

                var nameMatch = Regex.Match(fieldDeclaration, @"([^\s\{]+)\s*([=\{][\s\S]+)?\s*;$");
                infos[i].name = nameMatch.Groups[1].Value;
                infos[i].attributeInfos = ParseAttributes(match.Groups["attr"].Value);

                ++i;
            }

            return infos;
        }
    }
}
